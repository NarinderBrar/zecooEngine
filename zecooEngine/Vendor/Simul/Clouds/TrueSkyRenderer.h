#ifndef SIMUL_CLOUDS_TRUESKYRENDERER_H
#define SIMUL_CLOUDS_TRUESKYRENDERER_H

#include "Platform/Core/PropertyMacros.h"
#include "Platform/Core/ReadWriteMutex.h"
#include "Simul/Clouds/BaseWeatherRenderer.h"
#include "Platform/CrossPlatform/View.h"
#include "Platform/CrossPlatform/Camera.h"
#include "Platform/CrossPlatform/RenderDelegate.h"
#include "Platform/CrossPlatform/DemoOverlay.h"
#ifdef SIMUL_USE_SCENE
#include "Simul/Scene/BaseSceneRenderer.h"
#endif
#include "Simul/Terrain/BaseTerrainRenderer.h"
#include "Simul/Terrain/BaseWaterRenderer.h"
#include "Simul/Terrain/WaterParticleSimulator.h"
#include "Simul/Terrain/WaterParticleSimulator.h"
#include "Simul/Clouds/Export.h"
#include "Simul/Shaders/SL/sky_constants.sl"
#include "Platform/Shaders/SL/solid_constants.sl"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

namespace simul
{
	namespace crossplatform
	{
		class HdrRenderer;
	}
	namespace scene
	{
		class Scene;
		class BaseSceneRenderer;
		class Provider;
	}
	namespace clouds
	{
		class Skylight;
		enum PerformanceTestLevel
		{
			DEFAULT
			,FORCE_1920_1080
			,FORCE_2402_1461
			,FORCE_2560_1600
			,FORCE_3840_2160
		};
		enum DebugOverlays
		{
			NONE						=0x00000000
			,WATER_TEXTURES				=0x00000001
			,HDR_TEXTURES				=0x00000002
			,LIGHT_VOLUME				=0x00000004
			,GROUND_GRID				=0x00000008		
			,CUBEMAPS					=0x00000010
			,LIGHTNING_OVERLAY			=0x00000020
			,DIAGNOSTICS				=0x00000040
			,TRANSPARENCY_OVERLAY		=0x00000080
			,SPHERICAL_PLANET			=0x00000100
			,COMPOSITING				=0x00000200
			,CLOUD_CROSS_SECTIONS		=0x00000400
			,FADES						=0x00000800
			,RAIN_TEXTURES				=0x00001000
			,QUERIES					=0x00002000
			,CELESTIAL_DISPLAY			=0x00004000
			,CLOUD_EDIT_DISPLAY			=0x00008000
			,FLOW_RAYS					=0x00010000
			,CLOUD_AUXILIARY_TEXTURES	=0x00020000
			,CLOUD_QUERIES				=0x00040000
			,ONSCREEN_PROFILING			=0x00080000 //redundant in standalone due to profiler, but helpful in engine
			,AURORA						=0x00100000
		};
		SIMUL_BIT_FLAG(DebugOverlays, int)
		/*!

		*/
		class SIMUL_CLOUDS_EXPORT TrueSkyRenderer:public crossplatform::BaseRenderer
		{
		public:
			TrueSkyRenderer(simul::clouds::Environment *env,simul::scene::Scene *sc,simul::base::MemoryInterface *m,bool terrain=false);
			virtual ~TrueSkyRenderer();
			META_BeginProperties
				META_Property(bool,Paused						,"Whether rendering is paused - if so, real-time based elements will not animate.")
				META_Property(bool,ShowWater					,"Show water surfaces.")
				META_Property(bool,RenderSky					,"Whether to render the Sky or not")
				META_Property(bool,InfraRed						,"Whether to use infrared rendering.")
				META_Property(vec3,InfraRedIntegrationFactors	,"Multipliers to convert three spectral radiances to one radiance value in IR.")
				META_Property(bool,ShowFlares					,"Whether to draw light flares around the sun and moon.")
				META_Property(bool,ShowTerrain					,"Whether to draw the terrain.")
				META_Property(bool,UseHdrPostprocessor			,"Whether to apply post-processing for exposure and gamma-correction using a post-processing renderer.")
				META_RangeProperty(PerformanceTestLevel,PerformanceTest,DEFAULT,DEFAULT,FORCE_3840_2160,"Force rendering at a higher resolution to test performance");

				META_Property(float,SkyBrightness				,"Brightness of the sky (only).")
				META_PropertyWithSetCall(int,Antialiasing		,RecompileShaders,"How many antialiasing samples to use.")
				META_Property(int, MaxViewAgeFrames				,"How many frames without an update before a view is freed.")
				META_PropertyWithSetCall(int,OverlayViewId		,OverlayViewIdChanged,"Which view to show overlays for.")
			META_EndProperties
					
			void SetDebugOverlay(DebugOverlays d, bool v)
			{
				if (v)
					debugOverlays |= d;
				else
					debugOverlays &= (~d);
			}
			bool GetDebugOverlay(DebugOverlays d) const
			{
				return (debugOverlays&d)!=NONE;
			}
			DebugOverlays debugOverlays;
			crossplatform::RenderDelegate GetRenderDelegate()
			{
				return renderDelegate;
			}
			crossplatform::RenderDelegate renderDelegate;

			//! Platform-dependent function called when initializing the renderer.
			virtual void RestoreDeviceObjects(crossplatform::RenderPlatform *r);
			//! Platform-dependent function called when uninitializing the renderer.
			virtual void InvalidateDeviceObjects();
			virtual void ReverseDepthChanged(){}
			//! Check that all shaders have been correctly compiled
			static void EnsureEffectsAreBuilt(crossplatform::RenderPlatform *r);
			//! Platform-dependent function to reload the shaders - only use this for debug purposes.
			virtual void RecompileShaders();
			void	Render(crossplatform::GraphicsDeviceContext &deviceContext);
			void	RenderStandard(crossplatform::GraphicsDeviceContext &deviceContext,const crossplatform::CameraViewStruct &cameraViewStruct);
			//! Render the active debug overlays
			void	RenderOverlays(crossplatform::GraphicsDeviceContext &deviceContext,crossplatform::Texture *texture,float exposure,float gamma);
			void	ShowSkylights(crossplatform::GraphicsDeviceContext &deviceContext);
			void	RenderCubemap(crossplatform::GraphicsDeviceContext &parentDeviceContext,int cube_id,crossplatform::BaseFramebuffer *fb,int faceIndex,const mat4 &engineToSimulMatrix4x4,float blend,float exposure,float gamma);
			void	RenderEnvmap(crossplatform::GraphicsDeviceContext &deviceContext,crossplatform::BaseFramebuffer *cubemapFramebuffer);
			/// Update the specified texture as a cubemap, and its spherical harmonics.
			/// Copy the specified skylight into the target texture. This texture MUST be the same size, format and mip count as
			/// was specified for this cubemap id when GetSkylight was called.
			/// This function has latency depending on the platform.
			bool	CopySkylight(crossplatform::GraphicsDeviceContext &deviceContext,crossplatform::Texture *targetTexture,float *targetShValues,int shOrder,const mat4 *engineToSimulMatrix4x4,int updateFrequency,float blend,float exposure,float gamma,const vec3 &ground_colour
									,int amortization,bool allFaces,bool allMips);
			/// Create or update a skylight.
			void	EnsureSkylight(int cubemap_view_id,int size,int mips,crossplatform::PixelFormat format,const mat4 *cubeToSimulMatrix,int shOrder,int updateFrequency,float blend,float exposure,float gamma, const vec3 &ground_colour,int amortization,bool allFaces,bool allMips);
			/// Get linear colours from the specified skylight. Returns true if successful.
			bool ProbeSkylight(crossplatform::DeviceContext &pContext
				,int cubemap_view_id
				,int mip_size
				,int face_index
				,uint2 pos
				,uint2 size
				,vec4 *targetValuesFloat4);
			//! Reload the textures
			void	ReloadTextures();
			void	SetToolDir(int view_id	,vec3 t,float press,float szKm);
			void	AddView			(int);
			void	RemoveView		(int);
			void	ResizeView		(int view_id	,int w,int h);
			void	SetVRDistortion	(int view_id	,bool d);
			void	SetViewType		(int view_id	,crossplatform::ViewType vt);
			void	SetCamera		(int view_id	,const simul::crossplatform::CameraOutputInterface *c);
			/// How often to update each cloud texel; 1:always, 2:every 4 frames, n:every n^2 frames.
			void SetAmortization(int view_id	,int a);
			//! Get the amortization of the given frame
			int GetAmortization(int view_id	) const;
			//! Get a pointer to the water renderer.
			terrain::BaseWaterRenderer	*GetWaterRenderer()
			{
				return waterRenderer;
			}
			//! Get a pointer to the weather renderer.
			class clouds::BaseWeatherRenderer *GetSimulWeatherRenderer()
			{
				return weatherRenderer;
			}
			//! Get a pointer to the scene renderer.
			simul::scene::BaseSceneRenderer	*GetSceneRenderer()
			{
				return sceneRenderer;
			}
			//! Get a pointer to the terrain renderer.
			terrain::BaseTerrainRenderer *GetTerrainRenderer()
			{
				return baseTerrainRenderer;
			}
			//! Get a pointer to the HDR renderer.
			crossplatform::HdrRenderer *GetSimulHDRRenderer()
			{
				return simulHDRRenderer;
			}
			void	SaveScreenshot(const char *filename_utf8, int width = 0, int height = 0, float exposure = 1.0f, float gamma = 0.44f);
			void SetMemoryInterface(simul::base::MemoryInterface *m)
			{
				// TODO CHeck existing mi for allocations.
				memoryInterface=m;
			}
			// Fill the table of colours for a sky keyframe
			bool FillColourTable(sky::uid uid,int x,int y,int z,float *target);
			/// Delete GPU allocations to save memory. By default, max age is MaxViewAgeFrames. max_age=0 clears all views.
			void CleanupOldAllocations(int max_age=-1);
			/// Load sequence data.
			void SetSequence(const char *txt);
			//! Set the interpolation mode for cloud updates
			void SetInterpolationMode(int value = -1);
			void AsynchronousUpdate(crossplatform::DeviceContext &deviceContext);
		protected:
			void Lock(const char *) const;
			void Unlock() const;
			//! Once per-frame update. Do this before any rendering each frame.
			void PreRenderUpdate(crossplatform::GraphicsDeviceContext &deviceContext);
			void UpdateSkylights(crossplatform::GraphicsDeviceContext &deviceContext);
			int cubemapIndex;
			void RenderSelection(crossplatform::DeviceContext &deviceContext);
			void EnsureCorrectBufferSizes(int view_id);
			void RenderDepthBuffers(crossplatform::GraphicsDeviceContext &deviceContext,crossplatform::Texture *depthTexture,crossplatform::Viewport viewport,int x0,int y0,int w,int h);
			/// Render the sky.
			void	RenderMixedResolutionSky(crossplatform::GraphicsDeviceContext &deviceContext
									,crossplatform::Texture *depthTexture
									,const crossplatform::Viewport *depthViewport
									,float exposure
									,float gamma
									,float brightnessToUnity);
			virtual void	RenderDepthElements(crossplatform::GraphicsDeviceContext &deviceContext
										,float exposure
										,float gamma);
			/// Spectral radiance at r,g,b for output colour value 1.0. Multiply by output colour to get the real spectral radiance.
			float										ReferenceSpectralRadiance;
			crossplatform::RenderPlatform				*renderPlatform;
			crossplatform::HdrRenderer					*simulHDRRenderer;
		public:
			crossplatform::ViewManager	viewManager;
		protected:
			clouds::BaseWeatherRenderer					*weatherRenderer;

			simul::clouds::Environment					*environment;
			int											cubemap_view_id;
			std::string									screenshotFilenameUtf8;
			crossplatform::Effect						*linearizeDepthEffect;
			simul::terrain::BaseTerrainRenderer			*baseTerrainRenderer;
			simul::terrain::BaseWaterRenderer			*waterRenderer;
			simul::scene::BaseSceneRenderer				*sceneRenderer;
			simul::crossplatform::BaseFramebuffer		*envmapFramebuffer;
			simul::crossplatform::BaseFramebuffer		*msaaFramebuffer;
			simul::base::MemoryInterface				*memoryInterface;
			simul::crossplatform::Texture				*linearDepthTexture;
			simul::crossplatform::Texture *loss2DTexture;
			simul::crossplatform::Texture *inscatter2DTexture;
			simul::crossplatform::Texture *cloudVisibilityRT;
			simul::crossplatform::Texture *cloudShadowRT;
			simul::crossplatform::Texture* altitudeLightRT;
			simul::crossplatform::Texture *skylightCubemap;
			simul::crossplatform::Texture *reflectionCubemap;
			simul::crossplatform::Texture* waterCubemap;
			vec3 cloudShadowOriginKm;
			vec3 cloudShadowScaleKm;
			std::map<int,const simul::crossplatform::CameraOutputInterface *> cameras;
			crossplatform::DemoOverlay *demoOverlay;		
			platform::core::Timer timer;
			std::string profilingText;
			void RenderTransparentTest(crossplatform::GraphicsDeviceContext &deviceContext);
			crossplatform::Effect *transparentEffect;
			crossplatform::ConstantBuffer<SolidConstants> solidConstants;
			crossplatform::ConstantBuffer<SceneConstants> sceneConstants;
			crossplatform::Mesh *transparentMesh;
			vec3 toolDir;
			float toolPress;
			float toolSizeKm;
			float real_time;
			public:
			int maxCpuProfileLevel;
			int maxGpuProfileLevel;
			protected:
			int last_framenumber;
			bool render_truesky;
			bool reverse_depth;
			bool recompile_shaders;
			mutable simul::base::ReadWriteMutex mutex;
			std::map<std::string,crossplatform::Texture *> externalTextures;
			std::map<int,crossplatform::Texture*> cloudPlacementTextures;
			/// Should the calcrealtime action use a fixed time step?
			bool override_timestep;
			/// The fixed time step value (in seconds)
			float override_timestep_value;
			void OverlayViewIdChanged();
			void CycleOverlayViewId();
		public:
			bool onscreenProfiling;
			bool profilingEnabled;
			bool dumpProfilingText;
		protected:
			simul::crossplatform::DeviceContext *lastContext;

			std::map<int,Skylight *> skylights;
			std::map<int,int > skylightFaces;
			LightingQueryResult lightingQueryResult;
			int interpolationMode;
			vec3 sunlightTableTransform;
			float framerate=0.0f;
		};
		extern SIMUL_CLOUDS_EXPORT void ExportCloudLayer(simul::scene::BaseSceneRenderer *sceneRenderer,simul::scene::Provider *p,clouds::BaseWeatherRenderer *weatherRenderer,crossplatform::RenderPlatform *renderPlatform,const char *filenameUtf8);
	}
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif