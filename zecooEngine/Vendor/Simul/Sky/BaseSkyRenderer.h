#ifndef SIMUL_SKY_BASESKYRENDERER_H
#define SIMUL_SKY_BASESKYRENDERER_H

#include "Simul/Sky/SkyTexturesCallback.h"
#include "Simul/Sky/SkyKeyframer.h"
#include "Simul/Sky/Export.h"
#include "Platform/Core/ProfilingInterface.h"
#include "Platform/CrossPlatform/BaseRenderer.h"
#include "Platform/CrossPlatform/DeviceContext.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/sky_constants.sl"
#include "Simul/Shaders/SL/earth_shadow_uniforms.sl"
#include "Platform/CrossPlatform/Effect.h"
#include "Platform/CrossPlatform/Layout.h"

#include <map>
#include <set>

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
	#pragma warning(disable:4275)
#endif

namespace simul
{
	namespace math
	{
		struct float3;
	}
	namespace crossplatform
	{
		class RenderPlatform;
		class Texture;
		class Effect;
		class EffectTechnique;
		class EffectPass;
		class Buffer;
		struct PosColourVertex;
	}
	namespace sky
	{
		SIMUL_SKY_EXPORT_STRUCT PlanetStruct
		{
			crossplatform::Texture* pTexturePtr=nullptr;
			bool external_texture=false;
			vec3 dir;
			vec3 colour;
			float angular_radius=0.f;
			bool do_lighting=true;
			bool do_render=true;
		};
		SIMUL_SKY_EXPORT_STRUCT LocalFadeTextures
		{
			LocalFadeTextures();
			~LocalFadeTextures();
			void InvalidateDeviceObjects();
			void FreeGPUMemory();
			void RestoreDeviceObjects(crossplatform::RenderPlatform *renderPlatform);
			crossplatform::Texture*						loss_2d;
			crossplatform::Texture*						insc_2d;
			crossplatform::Texture*						skyl_2d;
			long long last_framenumber;
			long long max_age=7;
		};
		class AtmosphericScatteringInterface;
		class SiderealSkyInterface;
		class Sky;
		class SkyKeyframer;
		class FadeTableInterface;
		class OvercastCallback;
		class BaseGpuSkyGenerator;
		class Aurora;
		//! The base class for real-time sky renderers.

		/*! The Sky Renderer performs the following tasks:
		- Rendering stars: using RenderPointStars(void *context,float exposure)
		- Rendering planets (e.g. the Moon): using RenderPlanets(void *context,float exposure)
		- Rendering the sun, using RenderSun(void *context,float exposure)

		See \link simul::clouds::BaseWeatherRenderer::Render BaseWeatherRenderer::Render\endlink
		 and \link simul::clouds::BaseWeatherRenderer::RenderMixedResolution RenderMixedResolution\endlink.

		The rendering of the sky is no longer handled by BaseSkyRenderer or its derived classes because the
		\link simul::sky::BaseAtmosphericsRenderer atmospherics renderer\endlink now does this in a single pass including both sky and atmospherics.

		It is the job of the Sky Renderer to update the atmospherics textures to be used by the other render classes, and render celestial objects.
		
		The fade tables are stored as 3D textures - an axis for distance, one for elevation, one for altitude.
		The table size is given by NumElevations, NumDistances and SetNumAltitudes() of \link simul::sky::SkyKeyframer SkyKeyframer\endlink.
		Try to minimize the number of altitudes required, in order to maximize performance, and
		use SetAltitudeRangeKm on initialization to the range you will use.

		\link simul::clouds::BaseWeatherRenderer::PreRenderUpdate() BaseWeatherRenderer::PreRenderUpdate\endlink
		first calls \link simul::sky::BaseSkyRenderer::EnsureTexturesAreUpToDate EnsureTexturesAreUpToDate\endlink, which fills in the fade textures
		with loss and inscatter values.

		Then, the sky and atmospheric scattering (atmospherics, or distance-fades) are drawn together as an overlay
		after the depth-buffer has been filled by any solids in the scene
		(see \link simul::clouds::BaseWeatherRenderer::RenderMixedResolution\endlink).

		There are two main elements to realistic atmospherics: Loss, and Inscatter.

		\image html "FadeLogic.png" ""

		So, for any point in screen space, we can transform it to world space using the depth value, and obtain its distance, and elevation relative to the horizon. This gives us the
		lookup co-ordinates into the loss and inscatter textures. The loss value determines how much of the red,green, and blue values of that pixel are retained as light passes through the atmosphere from the distant object to the viewer.
		The inscatter determines how much sunlight (or moonlight etc) is scattered in towards the viewer due to the intervening atmosphere.

		Generally in Earth's nitrogen-oxygen atmosphere, more blue light than red or green is scattered. So distance white objects appear yellow, as the blue component has been scattered away. But distance black objects appear blue, because
		the blue part of the sunlight shining on the intervening air has been scattered as well, and some of that blue light goes towards the viewer. This process is Rayleigh scattering. Other important effects that TrueSky simulates are
		Mie scattering due to haze, Ozone absorption, and radiation due to air temperature.

		Each sky renderer maintains nine fade textures. If the most recent keyframe in the sky keyframer is X, we have:

		Keyframe X    | Keyframe X+1  | Keyframe X+2
		------------- | ------------- | -------------
		loss 0        | loss 1        | loss 2 
		inscatter 0   | inscatter 1   | inscatter 2 
		skylight 0    | skylight 1    | skylight 2 

		Each texture contains data for the atmospherics as seen from a complete range of altitudes.

		We use textures 0 and 1 to render the scene. Texture 2 is gradually filled in over multiple frames,
		so that when we reach keyframe X+1, it will be complete and can be swapped in.
		All this is handled automatically by the SkyKeyframer, which in turn can use a \link simul::sky::BaseGpuSkyGenerator GpuSkyGenerator\endlink.
		If using CPU-generated textures, the Sky Renderer asks the keyframer what new data to fill into the textures each frame. If using its own
		GpuSkyGenerator, the Sky Renderer has access to these textures without the need to query the keyframer.

		\image html "FadeTable.png" ""

		The fade textures are 3D, with co-ordinates representing x=altitude/max altitude, y=1+sine(elevation))/2, z=distance/max distance.
		Each frame, these three pairs of 3D textures (Keyframe X and X+1) are combined into three individual 2D textures, with
		x=distance/max distance, y=1+sine(elevation))/2, representing the fade colours as seen at the present time, from the current viewing altitude.

		The last texel in the x direction is considered to represent infinity, irrespective of the specified MaxDistanceKm parameter,
		so a distance texture coordinate of 1.0 in the inscatter and skylight textures represents the sky,
		with values less than one used for atmospheric scattering over solid objects.
*/
		SIMUL_SKY_EXPORT_CLASS BaseSkyRenderer : public simul::base::Referenced, public crossplatform::BaseRenderer
		{
		public:
			BaseSkyRenderer(SkyKeyframer *sk,simul::base::MemoryInterface *m);
			virtual ~BaseSkyRenderer();
			const BaseSkyRenderer &operator=(const BaseSkyRenderer &W);
			//! Get a pointer to the Sky Generator.
			virtual BaseGpuSkyGenerator *GetBaseGpuSkyGenerator()
			{
				return gpuSkyGenerator;
			}
			//! Get the results of a lighting query, and update it for next time. If this is the first call, the returned struct will have valid=false.
			LightingQueryResult GetLightingQuery(int id,const float pos[3]);

			//! Override this to create a custom generator.
			virtual BaseGpuSkyGenerator *CreateGpuSkyGenerator(base::MemoryInterface *m);
			//! Platform-dependent function to reload the textures - only use this for debug purposes.
			virtual void ReloadTextures();
			//! Check that all shaders have been correctly compiled
			static void EnsureEffectsAreBuilt(crossplatform::RenderPlatform *r) ;
			//! Platform-dependent function to reload the shaders - only use this for debug purposes.
			virtual void RecompileShaders();
			//! Platform-dependent function called when initializing the sky renderer.
			virtual void RestoreDeviceObjects(crossplatform::RenderPlatform *renderPlatform);
			//! Platform-dependent function called when uninitializing the sky renderer.
			virtual void InvalidateDeviceObjects();
			virtual void RenderBackgroundCubemap(crossplatform::GraphicsDeviceContext &, const crossplatform::ViewStruct &viewStruct,crossplatform::Texture *depthTexture,vec4 viewportTextureRegionXYWH,float exposure);
			//! Draw the stars.
			virtual bool RenderPointStars(crossplatform::GraphicsDeviceContext &, const crossplatform::ViewStruct &viewStruct, crossplatform::Texture *depthTexture, vec4 viewportTextureRegionXYWH,float,float,int );
			//! Draw the 2D fades to screen for debugging.
			virtual bool RenderFades(crossplatform::GraphicsDeviceContext &deviceContext,int view_id,int x,int y,int w,int h);
			//! Draw sidereal and geographic information to screen
			virtual void RenderCelestialDisplay(crossplatform::GraphicsDeviceContext &context,float y_heading=0.0f);
			//! Display LightingQueryResults to screen for debugging
			virtual void RenderLightingQueryResultsText(crossplatform::GraphicsDeviceContext& deviceContext, int x = 0, int y = 0);
			//! Save the current texture set to disk. Useful, but not necessary for normal operation.
			virtual void SaveTextures(const char *){}
			void SetSkyKeyframer(simul::sky::SkyKeyframer *s);
			//! Get a pointer to the fade table.
			simul::sky::SkyKeyframer *GetSkyKeyframer();
			//! Get the interface to the sky object so that other classes can use it for lighting, distance fades etc.
			simul::sky::BaseSkyInterface *GetBaseSkyInterface();
			const simul::sky::BaseSkyInterface *GetBaseSkyInterface() const;
			//! Get the sidereal sky interface - if using one, returns NULL otherwise.
			simul::sky::SiderealSkyInterface *GetSiderealSkyInterface();
			//! Inform the sky renderer of a callback to use to get overcast information.
			void SetOvercastCallback(simul::sky::OvercastCallback *ocb);

			//! Returns the current directional light colour as a float4 (x=red, y=green, z=blue, w unused), i.e. sunlight or moonlight. This is a high-dynamic range value.
			simul::sky::float4 GetLightColour(float alt_km);
			//! Get a value, from zero to one, which represents how much of the sun is visible.
			//! Call this when the current rendering surface is the one that has obscuring
			//! objects like mountains etc. in it, and make sure these have already been drawn.
			//! GetSunOcclusion executes a pseudo-render of an invisible billboard, then
			//! uses a hardware occlusion query to see how many pixels have passed the z-test.
			float CalcSunOcclusion(crossplatform::GraphicsDeviceContext &,float cloud_occlusion);
			//! Get a value, from zero to one, which represents how much of the sun is visible.
			float GetSunOcclusion() const{return sun_occlusion;}
			//! Draw the sun.
			virtual void RenderSun(crossplatform::GraphicsDeviceContext &, const crossplatform::ViewStruct &viewStruct, crossplatform::Texture *depthTexture, vec4 viewportTextureRegionXYWH, float exposure, crossplatform::Quaterniond q);
			//! Draw planets in the sky, e.g. the Moon.
			void RenderPlanets(crossplatform::GraphicsDeviceContext &, const crossplatform::ViewStruct &viewStruct,crossplatform::Texture *depthTexture, vec4 viewportTextureRegionXYWH, float exposure);
			//! This is called by RenderPlanets to render a planet with texture \a tex, angular radius \a rad radians, in direction \a dir (x,y,z), with colour \a colr.
			//! If \a do_lighting is true, the planet will be directionally-lit - e.g. moon phases.
			virtual void RenderPlanet(crossplatform::GraphicsDeviceContext &, const crossplatform::ViewStruct &viewStruct, crossplatform::Texture* tex, crossplatform::Texture *depthTexture, vec4 viewportTextureRegionXYWH, float rad, const float *dir, const float *colr, bool do_lighting,float exposure);
			//! Get the planet structure identified by \a index.
			PlanetStruct *GetPlanet(int index);
			//! Set the texture for planet \a index to \a tex
			void SetPlanetImage(uid u,crossplatform::Texture* tex,bool lighting,bool render);
			//! Get a texture that represents the variation of sunlight, moonlight, ambient and total directional light with altitude.
			crossplatform::Texture *GetLightTableTexture();
			//! Get the 2D texture that represents the extent of illuminated atmosphere visible in any given direction
			//! on a per-frame basis.
			crossplatform::Texture *GetIlluminationTexture();
			//! Get some per-frame text information for debugging - usually timing data.
			void SetBackgroundTexture(crossplatform::Texture *t);
			//! Override the Moon texture
			void SetMoonTexture(crossplatform::Texture *t);
			//! Draw the Aurora
			void RenderAurora(crossplatform::GraphicsDeviceContext& deviceContext, const crossplatform::ViewStruct& viewStruct, crossplatform::Texture* depthTexture, vec4 viewportTextureRegionXYWH, float exposure, crossplatform::Quaterniond& cloudWindowOrigin);
			void ShowAuroraTextures(crossplatform::GraphicsDeviceContext& deviceContext, int x0, int y0, int dx, int dy);

			LocalFadeTextures *GetLocalFadeTextures(int view_id);
			float GetBrightnessMultiplier() const
			{
				return brightnessMultiplier;
			}
		protected:
			//! Create a planet, identified by the \a index, which can be any unique number.
			//! The texture \a tex is an API-specific texture identifier, e.g. a GLuint, or LPDIRECT3D9TEXTURE.
			//! The angular radius will be \a rad, in radians. If \a do_lighting is true, the planet should be directionally lit by the sun
			//! to produce phases.
			void SetPlanet(uid u,const Moon &m,crossplatform::Texture* tex,bool do_lighting);
			void CreateGpuSkyGenerator();
			BaseGpuSkyGenerator *gpuSkyGenerator;
			crossplatform::Effect *effect;
			crossplatform::EffectTechnique*						techFade3DTo2D;
			crossplatform::EffectTechnique*						techSun;
			crossplatform::EffectTechnique*						techSunMSAA;

			crossplatform::EffectTechnique*						techQuery;
			crossplatform::EffectTechnique*						techFlare;
			crossplatform::EffectTechnique*						techPlanet;
			crossplatform::EffectTechnique*						techPlanetMsaa;
			crossplatform::EffectTechnique*						techUntexturedPlanet;
			crossplatform::EffectTechnique*						techPointStars;
			crossplatform::EffectTechnique*						techLightTableInterp;
			
			crossplatform::EffectTechnique*						techShowIlluminationBuffer;
			crossplatform::EffectTechnique*						techShowCrossSection;
			crossplatform::EffectTechnique*						techniqueShowLightTable;
			crossplatform::EffectTechnique*						techniqueShow2DLightTable;

			crossplatform::EffectTechnique*						techInterpLightTable;
			crossplatform::EffectTechnique*						techBackground;
			crossplatform::EffectTechnique*						techLightingQueries;
			crossplatform::EffectPass *passInterpLightTable;
			crossplatform::ConstantBuffer<SkyConstants>			skyConstants;
			crossplatform::ConstantBuffer<EarthShadowConstants>	earthShadowConstants;
			
			crossplatform::Texture						*loss_textures[3];
			crossplatform::Texture						*insc_textures[3];
			crossplatform::Texture						*skyl_textures[3];
			crossplatform::StructuredBuffer<vec4,crossplatform::BufferUsageHint::ONCE>		skyBrightnessSB[3];
			std::map<int,LocalFadeTextures*>			localFadeTextures;
			// Small framebuffers we render to once per frame to perform fade interpolation.
			crossplatform::Texture*						light_table_2d;
			crossplatform::Texture*						light_table;
			crossplatform::Texture*						illumination_2d;
			crossplatform::Texture*						background_texture;
			crossplatform::Texture*						overrideBackgroundTexture;

			crossplatform::ShaderResource				fadeTexture1;
			crossplatform::ShaderResource				fadeTexture2;
			crossplatform::ShaderResource				sourceTexture;
			crossplatform::ShaderResource				targetTexture;
	
			crossplatform::ShaderResource				lightTable2DTexture;
			crossplatform::ShaderResource				inscTexture;
			crossplatform::ShaderResource				lossTexture;
			
			crossplatform::ShaderResource				_depthTexture;
			crossplatform::ShaderResource				_depthTextureMS;
			crossplatform::ShaderResource				_flareTexture;
			crossplatform::ShaderResource				_lightingQueryInputs;
			crossplatform::ShaderResource				_lightingQueryResults;
			crossplatform::ShaderResource				_starsSB;
			crossplatform::ShaderResource				_globalBrightnessRW;
			crossplatform::ShaderResource				_skyBrightnessSB1;
			crossplatform::ShaderResource				_skyBrightnessSB2;
			crossplatform::Query						*sunQuery;	  

			void SetConstantsForPlanet(SkyConstants &skyConstants,const crossplatform::ViewStruct &viewStruct, const float *dir, float radiusRadians,float glowRadiusMultiple
				, const float *colour, const float *light_dir, vec4 viewportTextureRegionXYWH, float exposure);

			struct StarVertext
			{
				float x,y,z;
				float b;
			};
			simul::base::MemoryInterface *memoryInterface;
			crossplatform::RenderPlatform *renderPlatform;
			const float minimumStarBrightness;
			float brightnessMultiplier;
			float brightnessMultiplierCycle[3] = {0.0f, 0.0f, 0.0f};
			float interp_at_last_render;
			float gpu_time;
			float cpu_time;
			float texture_update_time;
			int texture_cycle;
			int num_stars;
			StarVertext *star_vertices;
			bool moon_enabled;
			std::map<uid,PlanetStruct> planets;
			simul::sky::SkyKeyframer *skyKeyframer;
			float sun_occlusion;
			int numAltitudes;
			int numFadeDistances,numFadeElevations;
			int moon_index;
			typedef std::map<int,LightingQueryResult> LightingQueryResults;
			LightingQueryResults lightingQueryResults;
			crossplatform::StructuredBuffer<LightingQueryResult> lightingQueryResultsSB;
			crossplatform::StructuredBuffer<vec4> lightingQueryInputs;
			crossplatform::Texture*			flare_texture;
			crossplatform::StructuredBuffer<vec4> starsSB;
			
			crossplatform::StructuredBuffer<vec4> globalBrightnessSB;
			void ClearIterators();
	// Make up to date with respect to keyframer:
			virtual void EnsureCorrectTextureSizes();
			void BuildStarsBuffer();
		public:
			//! Once per-frame update. Do this before any rendering each frame.
			virtual void PreRenderUpdate(crossplatform::GraphicsDeviceContext &deviceContext);
			//! Perform any necessary updates to the renderer's textures - particularly the loss, inscatter and skylight textures -
			//! at the start of a frame. This is called from simul::clouds::BaseWeatherRenderer::PreRenderUpdate().
			virtual void RenderIlluminationBuffer(crossplatform::GraphicsDeviceContext &deviceContext);
			//! Maintains the per-frame textures.
			virtual void EnsureTexturesAreUpToDate(crossplatform::GraphicsDeviceContext &deviceContext);
			// Get the inscatter colours from the given keyframe
			void GetColourTable(crossplatform::DeviceContext &deviceContext, const SkyKeyframe *K, int3 size, vec4 *target);
			//! This fills in the 2D atmospheric fade textures once per-frame from the keyframed 3D fade textures.
			virtual bool Render2DFades(crossplatform::GraphicsDeviceContext &deviceContext,int numDist=-1,int numElev=-1);
		protected:
			void Recompile();
			virtual void EnsureTextureCycle();
			virtual void CycleTexturesForward();
			//! Inform the derived class that the loss and inscatter textures \p texture_index, either 0, 1 or 2, at altitude
			//! index \p alt_index, (not used if not using multiple altitudes) should fill a block of texels
			//! starting at \a x, \a y, \a z, and \a w wide, \a l long, \a d deep, with the values in the array \a uint32_array,
			//! with the values in the arrays \p loss_float4_array and \p inscatter_float4_array, each of which contains
			//! four times \p num_texels floats. The first number is the value for \p texel_index, rather than the start of the whole table.
			//! Each group of four represents the r,g,b and a of a colour value, and the values may be greater
			//! than one, so floating point textures are preferred.
			//! Only one of FillFadeTexturesSequentially and FillFadeTextureBlocks need be implemented, depending on the graphics API.
			//! Some API's permit transfer of data sequentially in memory to textures (e.g. DirectX 9), some expect cuboid blocks of
			//! texels to be filled (e.g. OpenGL). The unwanted function should be implemented as a stub, with an assert() or some other
			//! fail condition, to make sure it is never called.
			virtual void FillFadeTextureBlocks(int ,int ,int ,int ,int ,int ,int ,const float *,const float *,const float *){}
			
		protected:
			long long last_update_framenumber;
			bool		initialized;
			unsigned	cloud_texel_index;
			void FillInLightingQueries(crossplatform::DeviceContext &deviceContext);
			virtual void CreateFadeTextures();
			bool GetSiderealTransform(float *world);
			void SetIlluminationConstants(EarthShadowConstants &earthShadowConstants,SkyConstants &skyConstants,const float *cam_pos);
			void HighlightConstellation(crossplatform::GraphicsDeviceContext &deviceContext,const std::string &str);
			crossplatform::PosColourVertex *lines;
			Aurora* aurora;
			float glow_mult=4.0f;
		public:
			static std::set<std::string> HighlightConstellations;
		};
	}
}

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#endif
