#pragma once
#include "Simul/Base/Referenced.h"
#include "Simul/Sky/Export.h"
#include "Simul/Sky/Float4.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/atmospherics_constants.sl"
#include "Simul/Shaders/SL/cloud_constants.sl"
#include "Simul/Shaders/SL/earth_shadow_uniforms.sl"
#include "Platform/CrossPlatform/Effect.h"
#include "Platform/CrossPlatform/BaseFramebuffer.h"
#include "Platform/CrossPlatform/AmortizationStruct.h"
#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

namespace simul
{
	namespace base
	{
		class ProfilingInterface;
	}
	namespace crossplatform
	{
		struct DeviceContext;
		class Texture;
		class Effect;
		class EffectTechnique;
		struct ViewStruct;
	}
	namespace sky
	{
		struct ScatteringVolume
		{
			ScatteringVolume():worldspaceInscatterTexture(NULL),lightspaceGodraysTexture(NULL),valid(false),godrays(true),initialized(false),last_subdivision_checksum(0)
			, last_framenumber(0), updateParameters(0, 0, 0, 0) {}
			~ScatteringVolume();
			crossplatform::Texture *worldspaceInscatterTexture;
			crossplatform::Texture *lightspaceGodraysTexture;
			crossplatform::Texture *tempTexture;
			mat4 worldToLightspaceMatrix;
			bool valid;
			bool godrays;
			bool initialized;
			unsigned last_subdivision_checksum;
			long long last_framenumber;
			int max_age=7;
			crossplatform::AmortizationStruct amortizationStruct;

			vec4 updateParameters;// like a checksum, but with 4 float values representing sun angle, altitude, haze.
			void FreeGPUMemory();
		};
		struct LocalFadeTextures;
		class SkyKeyframer;
		//! The graphics API-independent base class for atmospherics renderers, which draw distance fades, aerial perspective, fog and so on.
		SIMUL_SKY_EXPORT_CLASS BaseAtmosphericsRenderer
		{
		public:
			BaseAtmosphericsRenderer(simul::base::MemoryInterface *m);
			virtual ~BaseAtmosphericsRenderer();
			META_BeginProperties
				META_Property(bool,ShowGodrays					,"If true, draw the atmospheric fades with godrays.")
				META_Property(bool,InfraRed					,"Whether to use infrared rendering.")
				META_Property(uint3,GodraysGrid				,"Godrays detail: Azimuthal, radial, and longitudinal grid size.")
				META_Property(uint3,ScatteringGrid				,"Scattering detail: Azimuthal, elevation, and distance grid size.")
				META_RangeProperty(int,MaxAmortization	,4,1,8	,"Maximum amortization of scattering volume generation.")
			META_EndProperties
			//! Platform-dependent function called when initializing the atmospherics renderer.
			virtual void RestoreDeviceObjects(crossplatform::RenderPlatform *r);
			//! Platform-dependent function called when uninitializing the atmospherics renderer.
			virtual void InvalidateDeviceObjects();
			//! Once per-frame update. Do this before any rendering each frame.
			void PreRenderUpdate(crossplatform::GraphicsDeviceContext &);
			//! Set the texture defining the extent of illuminated inscatter. R=near, G=far, B and A unused.
			void SetIlluminationTexture(crossplatform::Texture *);
			//! Get the per-frame 3D volume scattering texture for the given view id.
			const ScatteringVolume *GetScatteringVolume(int view_id);
			//! Set the 2D cloud shadow texture to be used for lighting.
			virtual void SetCloudShadowTexture(const CloudShadowStruct &c)
			{
				cloudShadowStruct=c;
			}
			virtual void RenderLightspaceScatteringVolume(crossplatform::GraphicsDeviceContext &,bool cubemap,LocalFadeTextures *localFadeTextures,crossplatform::Texture *cubeDepthTexture,crossplatform::Texture *cubeUpdateTexture,bool godrays);
			//! Check that all shaders have been correctly compiled
			static void EnsureEffectsAreBuilt(crossplatform::RenderPlatform *r);
			//! Platform-dependent function to reload the shaders - only use this for debug purposes.
			virtual void RecompileShaders();
			//! Get the current location of the camera
			void SetCameraPosition(const float *pos);
			//! Set the sky and atmospherics interface to allow the renderer to use the correct lighting values.
			void SetSkyInterface(simul::sky::SkyKeyframer *si);
			void SetMaxFadeDistanceKm(float d){fade_distance_km=d;}
			//! Set the height of the fog
			void SetFogHeightKm(float h_km)
			{
				fog_height_km=h_km;
			}
			void SetFogColour(const float *c);
			void SetFogScaleHeightKm(float h_km)
			{
				fog_scale_height_km=h_km;
			}
			//! Set the density of the fog
			void SetFogDensity(float d)
			{
				fog_dens=d;
			}
			void SetAtmosphericsConstants(AtmosphericsUniforms &a);
			void SetAtmosphericsPerViewConstants(AtmosphericsPerViewConstants &a
													,float exposure
												 ,const crossplatform::ViewStruct &viewStruct
												,const vec4& relativeViewportTextureRegionXYWH
												,const vec4 &mixedResolutionTransformXYWH
												,const crossplatform::AmortizationStruct *amortizationStruct=NULL);
		protected:
			crossplatform::RenderPlatform *renderPlatform;
			float fade_distance_km;
			float fog_height_km;
			float fog_scale_height_km;
			float fog_dens;
			float4 fog_colour;
			SkyKeyframer *skyInterface;
			// For cloud shadows
			simul::base::MemoryInterface *memoryInterface;
			CloudShadowStruct cloudShadowStruct;
			float realtimeForwardOffset;
			std::map<int,ScatteringVolume*>					scatteringVolumes;

			crossplatform::Texture				*illuminationTexture;

			//! The HDR tonemapping hlsl effect used to render the hdr buffer to an ldr screen.
			crossplatform::Effect				*effect;

			crossplatform::ShaderResource		_lossTexture;
			crossplatform::ShaderResource		_inscTexture;
			crossplatform::ShaderResource		_skylTexture;
			crossplatform::ShaderResource		_illuminationTexture;
												
			crossplatform::ShaderResource		_depthTextureMS;

			crossplatform::ShaderResource		_depthTexture;

			crossplatform::ShaderResource		_cloudShadowTexture;
			crossplatform::ShaderResource		_cloudTexture;	

			
			crossplatform::ShaderResource		_targetVolume;
			
			
			crossplatform::ShaderResource		_cubeDepthTexture;
			crossplatform::ShaderResource		_cubeUpdateTexture;
			crossplatform::ShaderResource		_godraysTexture;
			crossplatform::ShaderResource		_cloudSamplerState;
			
			crossplatform::RenderState			*blendMultiply;
			crossplatform::RenderState			*blendAdd;

			crossplatform::ConstantBuffer<AtmosphericsPerViewConstants>	atmosphericsPerViewConstants;
			crossplatform::ConstantBuffer<AtmosphericsUniforms>			atmosphericsUniforms;
			crossplatform::ConstantBuffer<EarthShadowConstants>			earthShadowConstants;
			long long last_update_framenumber;
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
