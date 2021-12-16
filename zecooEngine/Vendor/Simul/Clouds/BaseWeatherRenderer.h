#ifndef SIMUL_CLOUDS_BASEWEATHERRENDERER_H
#define SIMUL_CLOUDS_BASEWEATHERRENDERER_H
#include "Simul/Clouds/Export.h"
#include "Platform/Core/PropertyMacros.h"
#include "Platform/Core/ProfilingInterface.h"

#include "Simul/Clouds/Environment.h"
#include "Platform/CrossPlatform/BaseFramebuffer.h"
#include "Platform/CrossPlatform/DeviceContext.h"
#include "Platform/CrossPlatform/BaseRenderer.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/compositing_constants.sl"
#include "Platform/CrossPlatform/Effect.h"
#include "Simul/Clouds/MixedResolutionCompositor.h"
#include <iostream>
#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif
struct LightingQueryResult;
namespace simul 
{
	namespace sky
	{
		class BaseSkyRenderer;
		struct float4;
		class SkyKeyframer;
		class BaseAtmosphericsRenderer;
		struct ScatteringVolume;
	}

	namespace crossplatform
	{
		class Effect;
		class EffectTechnique;
		class RenderPlatform;
		struct AmortizationStruct;
	}
	namespace clouds
	{
		class TwoResFramebuffer;
		struct TransparencyAtmospherics
		{
			crossplatform::Texture *nearFarTexture;
			crossplatform::Texture *inscatterVolumeTexture;
			crossplatform::Texture *loss2dTexture;
			crossplatform::Texture *renderedCloudTexture;
			vec4 fullResToLowResTransformXYWH;
			vec3 depthToLinFadeDistParams;
			float nearDist;
			float fogCeilingKm;
			float maxFadeDistanceKm;
			float fogExtinction;
			vec3 fogColour;
			vec3 fogAmbient;
			bool valid;
		};
		class CloudKeyframer;
		class BaseLightningRenderer;
		class CloudRenderer;
		class PrecipitationRenderer;

		/// The main base class for all weather renderers, such as SimulGLWeatherRenderer,
		/// SimulWeatherRenderer (DX11) etc.
		/// 
		/// The derived classes of BaseWeatherRenderer create and maintain the renderers for weather
		/// elements such as clouds, sky, rain and so on. So to use these sub-renderers it is usual to
		/// create an instance of one of the weather renderers, and allow this instance to manage the
		/// others.
		SIMUL_CLOUDS_EXPORT_CLASS BaseWeatherRenderer
		{
		public:
			//! Create a base weather renderer: if env is NULL, a new Environment will be created.
			static BaseWeatherRenderer* Create(Environment *env, simul::base::MemoryInterface *m = NULL);
			//! Destroy a base weather renderer.
			static void Destroy(BaseWeatherRenderer *baseWeatherRenderer);
			//! Default constructor: if env is NULL, a new Environment will be created.
			BaseWeatherRenderer(Environment *env,simul::base::MemoryInterface *m=NULL);
			virtual ~BaseWeatherRenderer();
		META_BeginProperties
			META_Property(float,FogAmbientBrightness												,"How much of the ambient light to use in colouring fog.")
			META_Property(float,FogLightBrightness													,"How much of the direct light to use in colouring fog (when fog is not in shadow).")
			META_Property(bool,DepthBlending														,"Whether to use depth-blending for clouds, if not, clouds are drawn behind or in front of scenery, depending on altitude.")
			META_Property(float,DepthSamplingPixelRange												,"Pixel range for sampling full-size depth buffer into cubemap.")
			META_RangePropertyWithSetCall(float,DefaultDownscale,1.f,32.f,DefaultDownscaleChanged	,"the downscale to be used for any new views. For existing views use GetFramebuffer(view_id) and set its Downscale, or use SetAllDownscale to override all.")
		META_EndProperties
			void DefaultDownscaleChanged();
		//! Override the downscale for all current views; does not affect the default - see SetDefaultDownscale.
			void SetAllDownscale(float);
			//! Create the member sub-objects: CloudRenderer etc.
			void CreateSubObjects();
			//! Platform-dependent function called when initializing the weather renderer.
			virtual void RestoreDeviceObjects(crossplatform::RenderPlatform *renderPlatform);
			//! Platform-dependent function called when uninitializing the weather renderer.
			virtual void InvalidateDeviceObjects();
			//! Ensure that per-view objects are destroyed for the view in question: they will be rebuilt if needed later.
			void RemoveView( int view_id);
			virtual void ReloadTextures();
			static void EnsureEffectsAreBuilt(crossplatform::RenderPlatform *renderPlatform);
			//! Platform-dependent function to reload the shaders - only use this for debug purposes.
			virtual void RecompileShaders();
			//! A transform for the cubemap set by SetCubemapTexture().
			void SetCubemapTransform(const float *m);
			//! A blurred texture, to be drawn usually per-frame.
			void SetBlurTexture(crossplatform::Texture *t);
			//!  This returns a struct containing the textures and values needed to apply atmospherics to transparent objects.
			TransparencyAtmospherics GetTransparencyAtmospherics(const crossplatform::ViewStruct &viewStruct);
			//! Get the view matrix in sidereal space, e.g. for rendering celestial objects.
			crossplatform::Quaterniond GetSiderealTransform();
			//! Draw the sun, moon and stars - call this while the depth buffer is bound.
			virtual void RenderCelestialBackground(crossplatform::GraphicsDeviceContext &deviceContext
				,const crossplatform::ViewStruct &viewStruct,TrueSkyRenderMode renderMode, crossplatform::Texture *depthTexture, vec4 viewportTextureRegionXYWH, float exposure);

			/// Render the sky including atmospherics, into the current rendertarget, using a supplied depth texture.
			///
			/// \em deviceContext is the platform-dependent render context,
			/// \em viewStruct2 is the view structure for alternate eye in VR,
			/// \em exposure is a multiplier for the rendered sky brightness,
			/// The \em view_id is an integer that distinguishes between multiple simultaneous viewports onscreen.
			/// By convention, viewport 0 is the main view, and viewport 1 is the cubemap for reflections and lighting.
			/// If \em is_cubemap is set, low-definition rendering is used.
			///	The \em mainDepthTexture is a single-sampled or MSAA depth texture. You can use a simul::crossplatform::Texture-derived
			/// class to wrapper your depth texture.
			/// The \em depthViewport determines what part of the depth texture represents this viewport - normally (0,0,1,1).
			/// Optional \em viewports specifies the left and right eye viewports if we're rendering both at once in VR.
			virtual void Render(crossplatform::GraphicsDeviceContext &deviceContext
													,const crossplatform::ViewStruct &viewStruct2
													,TrueSkyRenderMode renderMode
													,float exposure
													,float gamma
													,crossplatform::Texture* mainDepthTexture
													,crossplatform::Texture *cubemapTexture
													,const crossplatform::Viewport *depthViewport
													,const crossplatform::Viewport			*viewports=nullptr
													, vec3 cubemap_ground_colour=vec3(0,0,0)
													,int amortization=0);
			void RenderScreenspaceVisibility(crossplatform::GraphicsDeviceContext &deviceContext,crossplatform::Texture *targetTexture);

			/// Render the sky including atmospherics as an overlay, using a supplied platform-dependent depth texture.
			/// \em deviceContext is the platform-dependent render context, \em exposure is a multiplier for the rendered sky brightness,
			/// The \em view_id is an integer that distinguishes between multiple simultaneous viewports onscreen.
			/// By convention, viewport 0 is the main view, and viewport 1 is the cubemap for reflections and lighting.
			/// If \em is_cubemap is set, low-definition rendering is used.
			///	The \em mainDepthTexture is a single-sampled or MSAA depth texture. You can use a simul::crossplatform::Texture-derived
			/// class to wrapper your depth texture.
			///	The \em lowResDepthTexture is an API-dependent texture or texture resource pointer, with near far depth in the r and g, and b 0 or 1 for edges.
			/// The \em depthViewportXYWH determines what part of the depth texture represents this viewport - normally (0,0,1,1).
			/// Returns true if there is a lightpass.
			virtual bool RenderMixedResolution(	crossplatform::GraphicsDeviceContext &deviceContext
												,const crossplatform::ViewStruct &viewStruct2
												,crossplatform::Texture *depthTexture
												,TrueSkyRenderMode renderMode
												,float exposure
												,float gamma
												,const crossplatform::Viewport *depthViewports
												,crossplatform::Texture *ambientCubemapTexture);
			//! This composites the clouds and other buffers to the screen.
			virtual void CompositeCloudsToScreen(crossplatform::GraphicsDeviceContext &deviceContext
												,const crossplatform::ViewStruct &viewStruct2
												,TrueSkyRenderMode renderMode
												,float exposure
												,float gamma
												,bool depth_blend
												,crossplatform::Texture *mainDepthTexture
												,const vec4& viewportRegionXYWH
												,bool any_lightpass
												,const LightingQueryResult &lightingQueryResult
												,const vec3 &cubemap_ground_colour);
			virtual void RenderLightning(crossplatform::GraphicsDeviceContext &deviceContext,crossplatform::Texture *depth_tex
				,vec4 depthViewportXYWH,float brightnessToUnity);
			
			virtual void RenderPrecipitation(crossplatform::GraphicsDeviceContext &deviceContext
																,crossplatform::Texture *depth_tex
																,crossplatform::Texture *cubemapTexture
																,vec4 depthViewportXYWH
																,vec3 sunlight,vec3 moonlight, bool write_blend_alpha);
			//! Get lightning strikes. This calls GetExportLightningStrikes in CloudKeyframer, but also includes
			//! a cloud density query so that lightning can't come from empty sky.
			int GetExportLightningStrikes(ExportLightningStrike *export_strikes,int max_s,float game_time,float real_time) const;

			void RenderFramebufferDepth(crossplatform::GraphicsDeviceContext &deviceContext,int x0,int y0,int w,int h,int view_id=-1);
			void RenderCompositingTextures(crossplatform::GraphicsDeviceContext &deviceContext,int x0,int y0,int w,int h,int view_id=-1);
			void AsynchronousUpdate(crossplatform::DeviceContext &deviceContext,float real_time);
			//! Once per-frame update. Do this before any rendering each frame. For most platform implementations, you should set the
			//! matrices relevant to the \em main view before calling this.
			virtual void PreRenderUpdate(crossplatform::GraphicsDeviceContext &deviceContext,float real_time);
			//! Copy the properties of one BaseWeatherRenderer to another, including copying the properties of their sky and cloud renderers.
			const BaseWeatherRenderer &operator=(const BaseWeatherRenderer &W);
			//! Connect-up sky, clouds etc.
			virtual void ConnectInterfaces();
			//! Set a depth texture, whose alpha values represent the depth co-ordinate. This will be used when clouds are rendered in front of terrain etc.
			//virtual void SetDepthTexture(void *d);
			//! Get the representative HDR colour of the horizon, as seen from the specified altitude \param view_altitude_km .
			const simul::sky::float4 &GetHorizonColour(float view_altitude_km);
			//! Get the HDR light colour, as seen from the specified altitude \param view_altitude_km
			const simul::sky::float4 &GetLightColour(float view_altitude_km) const;
			//! Get a pointer to the sky renderer.
			simul::sky::BaseSkyRenderer *GetBaseSkyRenderer();
			//! Get a pointer to the lightning renderer.
			BaseLightningRenderer *GetLightningRenderer();
			//! Get a pointer to the rain/snow renderer.
			simul::clouds::PrecipitationRenderer *GetPrecipitationRenderer();
			//! Get a pointer to the cloud renderer.
			simul::clouds::CloudRenderer *GetBaseCloudRenderer()
			{
				return GetCloudRenderer();
			}
			simul::clouds::CloudRenderer *GetCloudRenderer();
			//! Get a pointer to the atmospheric renderer.
			simul::sky::BaseAtmosphericsRenderer *GetBaseAtmosphericsRenderer();
			//! Convenience function to get the next sky keyframe that can be modified without any recalculation.
			simul::sky::SkyKeyframer *GetSkyKeyframer();
			//! Get a pointer to the current Environment
			Environment *GetEnvironment();
			//! Set the Environment
			void SetEnvironment(Environment *env);
			crossplatform::Texture *GetCloudDepthTexture(int);
			void SaveCubemapToFile(const char *filename_utf8,float exposure,float gamma);
			// Virtual functions to create members or derived members:
			
			virtual sky::BaseSkyRenderer			*CreateSkyRenderer			(sky::SkyKeyframer *sk,base::MemoryInterface *m);
			virtual CloudRenderer				*CreateCloudRenderer		(CloudKeyframer *ck,base::MemoryInterface *m);
			virtual BaseLightningRenderer			*CreateLightningRenderer	(CloudKeyframer *ck,sky::SkyKeyframer *sk,base::MemoryInterface *m);
			virtual sky::BaseAtmosphericsRenderer	*CreateAtmosphericsRenderer	(base::MemoryInterface *m);
			virtual PrecipitationRenderer		*CreatePrecipitationRenderer(base::MemoryInterface *m);
			virtual Environment						*CreateEnvironment			(base::MemoryInterface *m);	
			bool HasFramebuffer(int view_id) const;
			// Map from view_id to framebuffer.
			typedef std::unordered_map<int,simul::clouds::TwoResFramebuffer*> FramebufferMap;
			virtual clouds::TwoResFramebuffer *GetFramebuffer(int view_id);
			FramebufferMap &GetFramebuffers()
			{
				return framebuffers;
			}
			/// Delete framebuffers that have not been used in max_age frames, to free GPU memory.
			void CleanUpFramebuffers(int max_age);
			MixedResolutionCompositor				mixedResolutionCompositor;

			void SetRealTimeWind(bool value);
		protected:
			FramebufferMap								framebuffers;			
			//! Renders the 2D clouds and atmospherics. If the passed depth texture is MSAA, near_pass determines whether to use the near or far depth for each depth texel.
			virtual bool RenderLowResolutionElements(crossplatform::GraphicsDeviceContext &deviceContext
															,float exposure
															,float godrays_strength
															,TrueSkyRenderMode renderMode
															,crossplatform::NearFarPass nearFarPass
															,crossplatform::Texture *lowResDepthTexture
															,const sky::ScatteringVolume *scatteringVolume
															,const vec4& viewportRegionXYWH
															,const crossplatform::AmortizationStruct &amortizationStruct
															,crossplatform::Texture *ambientCubemapTexture);
			float										dt_real_seconds;
			Environment									*environment;
			crossplatform::RenderPlatform				*renderPlatform;
			virtual void UpdateSkyAndCloudHookup();
			simul::clouds::BaseLightningRenderer		*baseLightningRenderer;
			simul::sky::BaseSkyRenderer					*baseSkyRenderer;
			simul::clouds::CloudRenderer			*baseCloudRenderer;
			simul::clouds::PrecipitationRenderer	*basePrecipitationRenderer;
			simul::sky::BaseAtmosphericsRenderer		*baseAtmosphericsRenderer;
			bool										RenderCloudsLate;
			float										last_time;
			float										last_real_time;
			long long									last_update_frame;
			simul::base::MemoryInterface				*memoryInterface;
			crossplatform::Effect						*effect;

			crossplatform::ShaderResource				_depthTextureMS;
			crossplatform::ShaderResource				_depthTexture;
			crossplatform::ShaderResource				_imageCubeArray;
			crossplatform::ShaderResource				_farImageTexture;
			crossplatform::ShaderResource				_nearImageTexture;
			crossplatform::ShaderResource				_nearFarTexture;
			crossplatform::ShaderResource				_lightpassTexture;
			crossplatform::ShaderResource				_shadowTexture;
			crossplatform::ShaderResource				_inscatterVolumeTexture;
			crossplatform::ShaderResource				_godraysVolumeTexture;
			crossplatform::ShaderResource				_loss2dTexture;
			
			simul::crossplatform::ConstantBuffer<CompositingConstants>	compositingConstants;

		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif
