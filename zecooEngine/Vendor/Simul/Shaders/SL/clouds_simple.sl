//  Copyright (c) 2007-2016 Simul Software Ltd. All rights reserved.
#ifndef CLOUDS_SIMPLE_SL
#define CLOUDS_SIMPLE_SL

RaytracePixelOutput RaytraceCloudsStatic(Texture3D cloudDensity
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
											,int numSteps
											,const int num_interp
											,bool do_godrays
											,bool consistent_steps)
{
	RaytracePixelOutput res;
	vec4 insc[NUM_CLOUD_INTERP];
	for(int ii=0;ii<num_interp;ii++)
	{
		res.colour[ii]			=vec4(0,0,0,1.0);
		insc[ii]				=vec4(0,0,0,0);
	}
	//vec4 colours[]={vec4(1.0,1.0,0.0,1.0),vec4(1.0,0.1,0.1,1.5),vec4(0.0,1.0,0.0,1.5),vec4(1.0,0.7,0.0,1.5),vec4(0.0,1.0,0.5,1.5)
	//	,vec4(0.0,0.0,1.0,1.5),vec4(0.8,0.2,1.0,1.5),vec4(0.8,0.6,0.2,1.5)};
	res.nearFarDepth		=dlookup;

	float s					=saturate((directionToSun.z+MIN_SUN_ELEV)/0.01);
	vec3 lightDirection		=lerp(directionToMoon,directionToSun,s);

	float cos0				=dot(lightDirection.xyz,view.xyz);
	float sine				=view.z;

	float min_z				=cornerPosKm.z-(fractalScale.z*1.5)/inverseScalesKm.z;
	float max_z				=cornerPosKm.z+(1.0+fractalScale.z*1.5)/inverseScalesKm.z;
/*	if(do_rain_effect)
		min_z				=-1.0;

	else if(view.z<-0.01&&viewPosKm.z<cornerPosKm.z-fractalScale.z/inverseScalesKm.z)
		return res;*/
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

	float n							=nfd.x;
	float f							=nfd.y;
	solidDist_nearFar[0]			=n;
	solidDist_nearFar[num_interp-1]	=f;
	for(int l=1;l<num_interp-1;l++)
	{
		float interp			=float(l)/float(num_interp-1);
		solidDist_nearFar[l]	=lerp(n,f,interp);
	}
	vec2 fade_texc			=vec2(0.0,0.5*(1.0-sine));
	// Lookup in the illumination texture.
	vec2 illum_texc			=vec2(atan2(view.x,view.y)/(3.1415926536*2.0),fade_texc.y);
	vec4 illum_lookup		=texture_wrap_mirror_lod(illuminationTexture,illum_texc,0);
	// TODO: reimplement illum if needed.
	vec2 nearFarTexc		=vec2(0,1.0);	//illum_lookup.xy;
	float meanFadeDistance	=1.0;
	float minDistance		=1.0;
	float maxDistance		=0.0;
	// Precalculate hg effects
	float BetaClouds		=lightResponse.x*HenyeyGreenstein(cloudEccentricity,cos0);
	float BetaRayleigh		=CalcRayleighBeta(cos0);
	float BetaMie			=HenyeyGreenstein(hazeEccentricity,cos0);

	float moisture			=0.0;

	vec3 world_pos			=viewPosKm;
	view.xyz				+=vec3(0.00001,0.00001,0.00001);
	// This provides the range of texcoords that is lit.
	// In c_offset, we want 1's or -1's. NEVER zeroes!
	int3 c_offset			=int3(2.0*step(vec3(0,0,0),view.xyz)-vec3(1,1,1));
	int3 start_c_offset		=-c_offset;
	start_c_offset			=int3(max(start_c_offset.x,0),max(start_c_offset.y,0),max(start_c_offset.z,0));
	vec3 viewScaled			=view/scaleOfGridCoordsKm;
	viewScaled				=normalize(viewScaled);

	vec3 offset_vec			=vec3(0,0,0);
	float maxDistKm			=maxCloudDistanceKm;//*sqrt(1.0-sine*sine)+1.0*sine;
	float K					=log(maxDistKm);
	float step_ratio		=1.0+K/float(numSteps);
	
	{
		float a		=1.0/(saturate(view.z)+0.00001);
		offset_vec	+=max(0.0,min_z-world_pos.z)*vec3(view.x*a,view.y*a,1.0);
	}
	
	{
		float a		=1.0/(saturate(-view.z)+0.00001);
		offset_vec	+=max(0.0,world_pos.z-max_z)*vec3(view.x*a,view.y*a,-1.0);
	}
	vec3 halfway					=0.5*(lightDirection-view);
	world_pos						+=offset_vec;
	
	float distanceKm				=length(offset_vec);

	float distScale					=0.6/maxFadeDistanceKm;
	bool found=false;
	float stepKm = K*(1.2 + distanceKm) / float(numSteps);
	/// (1.0 + 100.0*abs(view.z));

	vec3 amb_dir=view;
	uint steps=initialSteps-uint((1&stepPos)!=0);
	uint in_step=0,c=0;
	float rangeKm				=initialSteps*stepKm;

	float oddRangeKm				=(0.5*stepKm*initialSteps);
	for(int i=0;i<768;i++)
	{
		//world_pos					+=0.001*view;
		if((view.z<0&&world_pos.z<min_z)||(view.z>0&&world_pos.z>max_z)||distanceKm>maxCloudDistanceKm)//||solidDist_nearFar.y<lastFadeDistance)
			break;
		//stepKm						*=(1.0+K/float(numSteps));
		distanceKm					+=stepKm;
		float fade					=1.0;
		if(consistent_steps)
		{
			uint odd						=(steps-in_step)%2;
			float fade_up				=saturate((rangeKm-distanceKm)/oddRangeKm);
			fade					=(odd!=0)?fade_up:1.0;
			// doubles every P steps.
			if(in_step++==steps)
			{
				stepKm					*=2.0;
				c++;
				uint u					=(1<<c);
				steps					=initialSteps-uint((u&stepPos)!=0);
				in_step					=0;
				// =range doubles.
				rangeKm					=distanceKm+initialSteps*stepKm;
				oddRangeKm				=(0.5*stepKm*initialSteps);
			}
		}
		// We fade out the intermediate steps as we approach the boundary of a detail change.
		// Now sample at the end point:
		world_pos					+=stepKm*view;
		if(!consistent_steps)
			stepKm					*=step_ratio;

		vec3 cloudWorldOffsetKm		=world_pos-cornerPosKm;
		vec3 cloudTexCoords			=cloudWorldOffsetKm*inverseScalesKm;
		float fadeDistance			=saturate(distanceKm/maxFadeDistanceKm);

		// maxDistance is the furthest we can *see*.
		maxDistance					=max(fadeDistance,maxDistance);
		
		if(fade>0)
		{
			vec4 density = sample_3d_lod(cloudDensity, wwcSamplerState, cloudTexCoords, fadeDistance*0.0);
			//if(!found)
			{
			//	found = found || (density.z > 0);
			}
			//if(found)
			{
				vec3 noise_texc			=world_pos.xyz*noise3DTexcoordScale+noise3DTexcoordOffset;
				vec3 noise_texc2		=world_pos.xyz*noise3DTexcoordScale2+noise3DTexcoordOffset2;

				vec4 noiseval			=vec4(0,0,0,0);
				if(noise)//&&12.0*fadeDistance<4.0)
				{
					vec4 noiseval1			=density.x*texture_3d_wrap_lod(noiseTexture3D,noise_texc,1.0*fadeDistance);
					vec4 noiseval2			=density.x*texture_3d_wrap_lod(noiseTexture3D,noise_texc2,1.0*fadeDistance);
					noiseval=noiseval1*cos(.2*3.14159*noiseval2.x) + noiseval1.wzxy*sin(.2*3.14159*noiseval2.y);
				}
				vec4 light				=vec4(1,1,1,1);
				calcDensity(cloudDensity,cloudLight,cloudTexCoords,noiseval,fractalScale,0,density,light);
				density.z*=fade;
				if(do_rain_effect)
				{
					// The rain fall angle is used:
					float dm			=rainEffect*fade*GetRainAtOffsetKm(precipitationVolume,cloudWorldOffsetKm,inverseScalesKm);
					moisture			+=0.01*dm*light.x;
					density.z			=saturate(density.z+dm);
				}
				if(density.z>0)
				{
					if(noise)
					{
						vec3 worley_texc	=(world_pos.xyz+worleyOffsetKm)*worleyTexcoordScale;
						minDistance			=min(max(0,fadeDistance-density.z*stepKm/maxFadeDistanceKm), minDistance);
						vec4 worley			=texture_wrap_lod(smallWorleyTexture3D,worley_texc,0);
						float wo			=4*density.y*(worley.w-0.6)*saturate(1.0/(12.0*fadeDistance));//(worley.x+worley.y+worley.z+worley.w-0.6*(1.0+0.5+0.25+0.125));
						//density.z			=lerp(density.z,saturate(0.3+(1.0+alphaSharpness)*((density.z+wo)-0.3-saturate(0.6-density.z))),density.w);
						//density.z			=saturate(0.3+(1.0+alphaSharpness)*((density.z+wo)-0.3-saturate(0.6-density.z)));
						//density.z			=saturate(0.3+(1.0+alphaSharpness)*((density.z+wo)-0.3+saturate(density.z-0.6)));
						density.z			*=lerp(1.0,worley.w,saturate(1.0/(12.0*fadeDistance)));
						amb_dir				=lerp(amb_dir,worley.xyz,0.1*density.z);
					}
					float brightness_factor;
					fade_texc.x				=sqrt(fadeDistance);
					vec3 volumeTexCoords	=vec3(volumeTexCoordsXyC.xy,fade_texc.x);

					ColourStep(res.colour,insc, meanFadeDistance, brightness_factor
								,lossTexture, inscTexture, skylTexture, inscatterVolumeTexture
								,do_godrays, godraysVolumeTexture ,lightspaceScale, godraysTexCoords
								,lightTableTexture
								,density, light, distanceKm, fadeDistance
								,world_pos
								,cloudTexCoords, fade_texc, nearFarTexc
								,1.0, volumeTexCoords, amb_dir
								,BetaClouds, BetaRayleigh, BetaMie
								,solidDist_nearFar, noise, do_depth_mix, distScale,0, noiseval,1.0,alphaSharpness);
					if(res.colour[0].a*brightness_factor<0.003)
					{
						for(int o=0;o<num_interp;o++)
							res.colour[o].a = 0.0;
						meanFadeDistance=fadeDistance;
						break;
					}
				}
			}
		}
	}
	//res.nearFarDepth.y	=	max(0.00001,res.nearFarDepth.x-res.nearFarDepth.y);
	//res.nearFarDepth.z	=	max(0.0000001,res.nearFarDepth.x-meanFadeDistance);// / maxFadeDistanceKm;// min(res.nearFarDepth.y, max(res.nearFarDepth.x + distScale, minDistance));// min(distScale, minDistance);
	res.nearFarDepth.zw	=	vec2(meanFadeDistance,moisture);
//for(int i=0;i<num_interp;i++)
	// now lose inscatter due to godrays:
#ifdef AVERAGE_GODRAYS
	float gr=1.0;
	if(do_godrays)
	{
		vec3 offsetKm					=view*min(meanFadeDistance,res.nearFarDepth.x)*maxFadeDistanceKm;
		vec3 lightspaceOffset			=(mul(worldToScatteringVolumeMatrix,vec4(offsetKm,1.0)).xyz);
		float r							=length(lightspaceOffset);
		vec3 lightspaceVolumeTexCoords	=vec3(frac(atan2(lightspaceOffset.x,lightspaceOffset.y)/(2.0*SIMUL_PI_F))
			,0.5+0.5*asin(lightspaceOffset.z/r)*2.0/SIMUL_PI_F
			,r);
		vec4 godrays					=texture_3d_wcc_lod(godraysVolumeTexture,lightspaceVolumeTexCoords,0);
		gr								*=godrays.x;
	}
#endif
	for(int j=0;j<num_interp;j++)
		res.colour[j].rgb+=insc[j].rgb;//*gr;
	return res;
}
#endif
