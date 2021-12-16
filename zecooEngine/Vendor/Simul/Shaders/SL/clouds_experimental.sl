//  Copyright (c) 2007-2020 Simul Software Ltd. All rights reserved.
#ifndef CLOUDS_RAYTRACE_EXP_SL
#define CLOUDS_RAYTRACE_EXP_SL

#define WIDTH_KM (600.0)
#define ANGLE_RANGE (0.0938)
#define ALTITUDE_RANGE_KM (20.0)
#define PLANET_RADIUS (6378.0)

//#define NON_LINEAR 1

#ifdef NON_LINEAR
#if 1

vec3 WorldToVolumePos(vec3 world_pos_km, vec3 centreVolumePos, vec3 vol_km)
{
	float R=PLANET_RADIUS;
	float x=world_pos_km.x;
	float y=world_pos_km.y;
	float h=R+world_pos_km.z;
	// from - to + 1, out of 300km.
	float theta_x=atan2(x,h);
	float theta_y=atan2(y,h);
	// Altitude out of 20km
	float r=sqrt(h*h+x*x+y*y);
	vec3 vol_offset = vec3(theta_x,theta_y,r-R)/angleRadiansAltitudeKmRange;
	return centreVolumePos + vol_offset;
}

vec3 VolumeToWorldPos(vec3 volume_pos, vec3 centreVolumePos, vec3 vol_km)
{
	float R=PLANET_RADIUS;
	vec3 vol_offset = volume_pos - centreVolumePos;
	vec3 vp= vol_offset *angleRadiansAltitudeKmRange;
	float r=R+vp.z;
	// from - to + 1, out of 300km.
	vec2 xy=sin(vp.xy)*r;
//	use Maclaurin expansion for sqrt.
	float h=sqrt(r*r-dot(xy,xy));
	// Altitude out of 20km
	return vec3(xy,h-R);
}
#else
//linearized:
// Volume position is +-1 in the x and y, 0-1 in the z.
vec3 WorldToVolumePos(vec3 world_pos_km, vec3 centreVolumePos, vec3 vol_km)
{
	float R=PLANET_RADIUS;
	float z=world_pos_km.z;
	float w=z+0.5*dot(world_pos_km,world_pos_km/R);
	float a=w-0.25*w*w/R;
	vec3 vp=vec3(world_pos_km.xy/R,a)/angleRadiansAltitudeKmRange;
	return vp + centreVolumePos;
}

vec3 VolumeToWorldPos(vec3 volume_pos, vec3 centreVolumePos, vec3 vol_km)
{
	vec3 vol_offset = volume_pos - centreVolumePos;
	float R=PLANET_RADIUS;
	vec3 vp=vol_offset*angleRadiansAltitudeKmRange;
	float z=vp.z;
	float r=R+z;
	// from - to + 1, out of 300km.
	float x=vp.x*r;//sin(theta_x)*r;
	float y=vp.y*r;//sin(theta_y)*r;
	vec3 xyz=vec3(x,y,z);
	float w=z + 0.5*(z*z-x*x-y*y)/R;
	float h=w-0.25*w*w/R;
	return vec3(x,y,h);
}
#endif
vec3 GridToWorldPos(vec3 grid_pos, vec3 windowGridf, vec3 centreVolumePos, vec3 vol_km)
{
	return VolumeToWorldPos(grid_pos / windowGridf, centreVolumePos, vol_km);
}
#else
// Volume position is +-.5 in the x and y, 0-1 in the z.
vec3 WorldToVolumePos(vec3 world_pos_km, vec3 centreVolumePos, vec3 vol_km)
{
	vec3 vp = world_pos_km / vol_km;
	return vp+centreVolumePos;
}

vec3 VolumeToWorldPos(vec3 volume_pos, vec3 centreVolumePos, vec3 vol_km)
{
	vec3 vp=volume_pos-centreVolumePos;
	vec3 wp = vp * vol_km;
	return wp;
}
vec3 GridToWorldPos(vec3 grid_pos, vec3 windowGridf, vec3 centreVolumePos, vec3 vol_km)
{
	vec3 wp=(grid_pos * vol_km/ windowGridf)-centreVolumePos* vol_km;
	return wp;
}
vec3 GridToWorldPos(vec3 grid_pos, vec3 grid_km, vec3 centreVolumePosKm)
{
	vec3 wp=grid_pos * grid_km-centreVolumePosKm;
	return wp;
}
#endif

// Grid pos is -511 to +512, and 0-64.
vec3 VolumeToGridPos(vec3 volume_pos,vec3 windowGridf,vec3 centreVolumePos)
{
	return windowGridf*(volume_pos+centreVolumePos);
}

vec3 GridToVolumePos(vec3 grid_pos,vec3 windowGridf,vec3 centreVolumePos)
{
	return grid_pos/windowGridf-centreVolumePos;
}

int3 FloatToIntGrid(vec3 grid_pos)
{
	return int3(floor(grid_pos));
}


vec3 WorldToGridPos(vec3 world_pos_km, vec3 windowGridf, vec3 centreVolumePos, vec3 vol_km)
{
	//WorldToVolumePos is world_pos_km / vol_km+centreVolumePos;
	vec3 vp=WorldToVolumePos(world_pos_km,centreVolumePos,vol_km);
	return windowGridf*vp;
}

vec3 WorldToGridPos(vec3 world_pos_km, vec3 gridKm, vec3 centreGridPos)
{
	return world_pos_km/gridKm+centreGridPos;
}

RaytracePixelOutput RaytraceCloudsExperimental(Texture3D cloudDensity
											,Texture3D cloudLight
											,Texture3D precipitationVolume
											,Texture3D noiseTexture3D
											,Texture2D lightTableTexture
											,Texture2D illuminationTexture
											,Texture2D lossTexture
											,Texture2D inscTexture
											,Texture2D skylTexture
											,Texture3D inscatterVolumeTexture
											,Texture3D godraysVolumeTexture
											,bool do_depth_mix
											,vec4 dlookup
											,vec3 view
											,vec4 clip_pos
											,vec3 volumeTexCoordsXyC
											,bool noise
											,bool do_rain_effect
											,bool do_rainbow
											,vec3 cloudIrRadiance1
											,vec3 cloudIrRadiance2
											,const int num_interp
											,bool do_godrays
											,vec3 cloudTintColour
											,uint seed)
{
	RaytracePixelOutput res;
	vec4 insc[NUM_CLOUD_INTERP];
	for(int ii=0;ii<num_interp;ii++)
	{
		res.colour[ii]			=vec4(0,0,0,1.0);
		insc[ii]				=vec4(0,0,0,0);
	}
	res.nearFarDepth		=dlookup;
	float s					=saturate((directionToSun.z+MIN_SUN_ELEV)/0.01);
	vec3 lightDirection		=lerp(directionToMoon,directionToSun,s);

	float cos0				=dot(lightDirection.xyz,view.xyz);
	float min_z				=cornerPosKm.z-(fractalScale.z*1.5)/inverseScalesKm.z;
	float max_z				=cornerPosKm.z+(1.0+fractalScale.z*1.5)/inverseScalesKm.z;
	if(do_rain_effect)
		min_z				=-1.0;
	//else if(view.z<-0.01&&viewPosKm.z<cornerPosKm.z-fractalScale.z/inverseScalesKm.z)
	//	return res;
	vec3 godraysTexCoords	=vec3(0,0,0);
	float lightspaceScale	=1.0;
	if(do_godrays)
	{
		//vec3 offsetKm					=view*min(meanFadeDistance,res.nearFarDepth.x)*maxFadeDistanceKm;
		// lightspaceView is a non-unit vector. Its length is the scaling from km to lightspace distance texcoord.
		vec3 lightspaceView				=(mul(worldToScatteringVolumeMatrix,vec4(view,1.0)).xyz);
		lightspaceScale					=length(lightspaceView);
		godraysTexCoords				=vec3(frac(atan2(lightspaceView.x,lightspaceView.y)/(2.0*SIMUL_PI_F)),0.5+0.5*asin(lightspaceView.z/lightspaceScale)*2.0/SIMUL_PI_F,0);
	}

	float solidDist_nearFar	[NUM_CLOUD_INTERP];
	vec2 nfd				=(dlookup.yx)+100.0*step(vec2(1.0,1.0), dlookup.yx);

	solidDist_nearFar[0]					=	nfd.x;
	solidDist_nearFar[num_interp-1]			=	nfd.y;
	for(int l=1;l<num_interp-1;l++)
	{
		float interp			=1.0-float(l)/float(num_interp-1);
		solidDist_nearFar[l]	=lerp(solidDist_nearFar[0],solidDist_nearFar[num_interp-1],1.0-interp);
	}
	float sine				=view.z;
	vec2 fade_texc			=vec2(0.0,0.5*(1.0-sine));
	// Lookup in the illumination texture.
	vec2 illum_texc			=vec2(atan2(view.x,view.y)/(3.1415926536*2.0),fade_texc.y);
	vec4 illum_lookup		=texture_wrap_mirror(illuminationTexture,illum_texc);
	vec2 nearFarTexc		=illum_lookup.xy;
	float meanFadeDistance	=1.0;
	// Precalculate hg effects
	float BetaClouds		=HenyeyGreenstein(cloudEccentricity,cos0);
	float BetaRayleigh		=CalcRayleighBeta(cos0);
	float BetaMie			=HenyeyGreenstein(hazeEccentricity,cos0);

	float moisture			=0.0;
	vec3 world_pos_km		=viewPosKm;
	world_pos_km.xy			-=windOffsetKm.xy;

	// start with the lowest-detail mip.
	uint mip				=uint(maxMip);
	uint mipscale			=uint(1)<<mip;
	uint3 mip_grid			=uint3(windowGrid)/mipscale;
	vec3 windowGridf		=vec3(mip_grid);
	vec3 centreVolumePos	=vec3(((gridCentreTexel.xy+int2(windowGrid.xy/2))%int2(windowGrid.xy)-int2(windowGrid.xy/2))/float(windowGrid.x),0);
	
	vec3 gridKm				=VolumeScaleKm/ windowGridf;
	vec3 centreVolumePosKm	= VolumeScaleKm*centreVolumePos;
	vec3 centreGridPos		= windowGridf*centreVolumePos;
	// world_pos_km is relative to the local origin.
	// The grid is centred at the local origin, but is spherical.
	vec3 grid_pos			=WorldToGridPos(world_pos_km, gridKm, centreGridPos);
	vec3 grid_dir			=normalize(WorldToGridPos(world_pos_km+view,gridKm, centreGridPos)-grid_pos);
	
	// In c_offset, we want 1's or -1's. NEVER zeroes!
	int3 c_offset			=int3(2.0*step(vec3(0,0,0),view.xyz)-vec3(1,1,1));

	// volume_dir is unit view direction vector in grid units.
	// unitStepKm is therefore the distance in km for a unit step in texel space - in the current view direction.
	int3 c_next				=FloatToIntGrid(grid_pos);
	uint idx				=0;
	int count_detail=0;
	vec3 amb_dir			=view;
	float distScale			=0.6/maxFadeDistanceKm;
	int i=0;
	uint use_mip=0;
	int steps_at_mip=0;
	vec3 last_grid_pos	=grid_pos;
	int3 c_offset_x		=int3(step(vec3(0,0,0),grid_dir.xyz));
	c_offset			=2*c_offset_x-int3(1,1,1);
	float distanceLimitKm = 0.f; // Distance from the viewpoint to the current position of the ray
	//if(view.z>0)
	//	return res;
	for(i=0;i<768;i++)
	{
		if((view.z<0&&world_pos_km.z<min_z)||(view.z>0&&world_pos_km.z>max_z) || distanceLimitKm > maxCloudDistanceKm)
			break;
		// Next pos.
		vec3 prev_world_pos_km	=world_pos_km;
		int3 c					=c_next;
		if(mip>use_mip)
		{
			// We're in the SKIPPING mode, at mip M.
			// We look in our current texel to see if there are clouds. The index is obtained by dividing the current gridpoint by 2, mip times.
			vec4 lookup				=vec4(0,0,0,0);
			int4 lookup_index		=int4((c.xy+mip_grid.xy)%mip_grid.xy,c.z,mip);
			if(c.z<mip_grid.z&&c.z>=0)
				lookup				=cloudDensity.Load(lookup_index);
			if(do_rain_effect)
				lookup.z			+=rainEffect*GetPrecipitationAtOffsetKm(precipitationVolume,(world_pos_km-cornerPosKm),inverseScalesKm,float(mip)).w;

			// if z is zero, we step forward to the next edge at the SAME mip level, then continue.
			// If z is not zero, we step down one mip level: if not at mip zero we test again.
			//												 but if we're now at mip zero, we start to render.
			// We're in the RENDERING mode, at mip zero.
			// world_pos_km is on a texel edge. We step to the next texel edge.
			// recalculate volume_dir based on current position:
			if(lookup.z!=0)
			{
			// we DO have clouds somewhere in here, so we have to step up a mip.
				mip--;
				mipscale			=uint(1)<<mip;
				mip_grid			=int3(windowGrid)/mipscale;
				windowGridf			=vec3(mip_grid);
				gridKm				=VolumeScaleKm/ windowGridf;
				centreVolumePosKm	=VolumeScaleKm*centreVolumePos;
				centreGridPos		=windowGridf*centreVolumePos;
				grid_pos			=WorldToGridPos(world_pos_km,gridKm, centreGridPos);
				c_next				=FloatToIntGrid(grid_pos);
				continue;
			}

			// Add 1 in all directions:
			int3 c1				=c+c_offset_x;
			//find the correct step:
			vec3 grid_pos_1		=c1;
			// dp=distances in x y and z to next wall.
			vec3 dp				=grid_pos_1-grid_pos;
			// The distance in gridspace from grid_pos to each wall is given by dividing dp on each axis by the directional component.
			// so D is three distances in gridspace, of which we seek the smallest.
			vec3 D				=dp/grid_dir;
			// e is the smallest gridspace distance from grid_pos to a wall.
			float e				=min(min(D.x,D.y),D.z);
			// All D components are positive. Only the smallest is equal to e. Step(x,y) returns (y>=x).
			// So step(D.x,e) returns (e>=D.x), which is only true if e==D.x

			// Thus we obtain N, which is 1 only in the direction to the nearest wall.
			vec3 N				=step(D,vec3(e,e,e));
			int3 c_step			=c_offset*int3(N);
			// This is the Step-forward
			//last_grid_pos		=grid_pos;
			grid_pos			+=e*grid_dir;
			c_next				=c+c_step;
			// What offset was the original position from the centre of the cube?
			// vec3 gridKm,  centreVolumePosKm
			world_pos_km		=GridToWorldPos(grid_pos,gridKm,  centreVolumePosKm);
			vec3 step_km		=world_pos_km-prev_world_pos_km;
			float stepKm		=length(step_km);
			distanceLimitKm =  length(world_pos_km - viewPosKm);//stepKm;
			continue;
		}

		// Add 1 in all directions:
		int3 c1				=c+c_offset_x;
		//find the correct step:
		vec3 grid_pos_1		=c1;
		// dp=distances in x y and z to next wall.
		vec3 dp				=grid_pos_1-grid_pos;
		// The distance in gridspace from grid_pos to each wall is given by dividing dp on each axis by the directional component.
		// so D is three distances in gridspace, of which we seek the smallest.
		vec3 D				=dp/grid_dir;
		// e is the smallest gridspace distance from grid_pos to a wall.
		float e = min(min(D.x, D.y), D.z);
		// All D components are positive. Only the smallest is equal to e. Step(x,y) returns (y>=x).
		// So step(D.x,e) returns (e>=D.x), which is only true if e==D.x

		// Thus we obtain N, which is 1 only in the direction to the nearest wall.
		vec3 N				=step(D,vec3(e,e,e));
		int3 c_step			=c_offset*int3(N);
		// This is the Step-forward
		//last_grid_pos		=grid_pos;
		grid_pos			+=e*grid_dir;
		c_next				=c+c_step;
		// What offset was the original position from the centre of the cube?
		// vec3 gridKm,  centreVolumePosKm
		world_pos_km		=GridToWorldPos(grid_pos,gridKm, centreVolumePosKm);
		vec3 step_km		=world_pos_km-prev_world_pos_km;
		float stepKm		=length(step_km);
	
		idx=uint(mip);
		vec3 sample_pos_km		=world_pos_km;
		vec3 cloud_tex_coords	=WorldToVolumePos(sample_pos_km,centreVolumePos, VolumeScaleKm);
		{
			// NOTE: VERY VERY IMPORTANT to use the original, not noise-modified, texture-coordinates for light.
			vec4 light = sample_3d_lod(cloudLight, wwcSamplerState, cloud_tex_coords, 0);
			vec3 noise_texc		=(sample_pos_km.xyz+windOffsetKm.xyz)*noise3DTexcoordScale+noise3DTexcoordOffset;
			vec4 noiseval		=vec4(0,0,0,0);
			if(noise)
			{
				vec4 noiseval1		=texture_3d_wrap_lod(noiseTexture3D,noise_texc,0.0);
				noiseval			=light.z*noiseval1;
			}
		
			vec3 noise_adjusted_texc= cloud_tex_coords.xyz + fractalScale.xyz * (noiseval.xyz);
			vec4 density = sample_3d_lod(cloudDensity, wwcSamplerState, noise_adjusted_texc, use_mip);
			
			if(do_rain_effect)
			{
				// The rain fall angle is used:
				float dm			=rainEffect*GetRainAtOffsetKm(precipitationVolume,(sample_pos_km-cornerPosKm)+((fractalScale.xyz*noiseval.xyz)/inverseScalesKm),inverseScalesKm);
				dm					*= saturate(0.6+texture_wrap_lod(noiseTexture3D,noise_texc+vec3(0,0,rainVerticalTexcoord),0.0).z);
				moisture			+=0.01*dm*light.x;
				density.z 			=max(density.z,dm);
			}
			
			//density.z=1.0;
			if(density.z>0)
			{
				float distanceKm		=length(world_pos_km-viewPosKm);
				float fadeDistance		=saturate(distanceKm/maxFadeDistanceKm);
				density.z				*=saturate(distanceKm/CLOUD_FADEIN_DIST);
				density.z				=lerp(1.0/(1.0+exp(-20.0*(density.z-0.3))),density.z,density.y);
				vec3 worley_texc		=(sample_pos_km.xyz+windOffsetKm.xyz)*worleyTexcoordScale+worleyOffsetKm;
				vec4 worley				=texture_wrap_lod(smallWorleyTexture3D,worley_texc,0);
				float wo				=4*light.w*lerp(1.0,worley.w,saturate(1.0/(12.0*fadeDistance)));
				density.z				=lerp(density.z,density.z*saturate(0.3+((density.z+wo)-0.3-saturate(0.6-density.z))),density.y);
				
				float brightness_factor;
				fade_texc.x				=sqrt(fadeDistance);
				vec3 volumeTexCoords	=vec3(volumeTexCoordsXyC.xy,fade_texc.x);
					
				float opacity = density.z;
				opacity = saturate(0.3+(1.0+1.0*density.w*alphaSharpness)*(opacity-0.3));
				float cosine = dot(N, abs(view));
				
				ColourStep2( res.colour, insc,  brightness_factor
							,inscatterVolumeTexture, lossTexture
							,do_godrays, godraysVolumeTexture ,lightspaceScale, godraysTexCoords
							,lightTableTexture
							,density, light, distanceKm, fadeDistance
							,world_pos_km
							,cloud_tex_coords, fade_texc
							,volumeTexCoords
							,BetaClouds 
							,solidDist_nearFar, do_depth_mix,distScale,idx,opacity,cosine,cloudTintColour);
			
				meanFadeDistance		=lerp(min(fadeDistance,meanFadeDistance), meanFadeDistance,(1.0-.4*density.z));
				if(res.colour[0].a*brightness_factor<0.03) 
				{
					for(int o=0;o<num_interp;o++)
					{
						res.colour[o].a = 0.0;
					}
					meanFadeDistance=fadeDistance;
					break;
				}
				continue;
			}
			
			count_detail++;
			if(count_detail>2)
			{
			// No clouds any more? Step back to top mip
				mip						=maxMip;
				mipscale				=int(1)<<mip;
				mip_grid				=int3(windowGrid)/mipscale;
				windowGridf				=vec3(mip_grid);
				gridKm					=VolumeScaleKm/ windowGridf;
				centreGridPos			=windowGridf*centreVolumePos;
				grid_pos				=WorldToGridPos(world_pos_km,gridKm, centreGridPos);
				c_next					=FloatToIntGrid(grid_pos);
				count_detail=0;
			}
		}
	}
	res.nearFarDepth.zw	=vec2(meanFadeDistance,moisture);
	float grey=float(1)/100.0;
	for(int k=0;k<num_interp;k++)
	{
		res.colour[k].rgb+=insc[k].rgb;
		//vec3 colours[]={{1,0,0},{0,1,0},{0,0,1},{1,1,0},{0,1,1},{1,0,1},{1,1,1}};
		//res.colour[k].rgb=grey;
		//res.colour[k].r=BetaClouds;
		//res.colour[k].rgb*=upsampling_offset;
		//res.colour[k].rgb=colours[min((i/10),7)]*(1.0-res.colour[k].a);
		//res.colour[k].a = 0.0;
					
	}
	return res;
}
#endif
