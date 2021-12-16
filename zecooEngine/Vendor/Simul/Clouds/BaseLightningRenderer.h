#ifndef SIMUL_CLOUDS_BASELIGHTNINGRENDERER_H
#define SIMUL_CLOUDS_BASELIGHTNINGRENDERER_H
#include "Simul/Clouds/Export.h"
#include "Simul/Clouds/CloudKeyframer.h"
#include "Platform/CrossPlatform/Effect.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/lightning_constants.sl"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

namespace simul
{
	namespace sky
	{
		struct float4;
	}
	namespace crossplatform
	{
		struct DeviceContext;
		class Texture;
		class Effect;
		class Layout;
		class Buffer;
	}
	namespace clouds
	{
        class BaseWeatherRenderer;
		struct LightningIllumination
		{
			vec3    centreKm;
			float   brightness;
			vec3    colour;
		};
		struct TransparencyAtmospherics;
		//! The renderer for lightning.
		SIMUL_CLOUDS_EXPORT_CLASS BaseLightningRenderer
		{
		public:
			//! Constructor
			                        BaseLightningRenderer(simul::sky::BaseSkyInterface *sk);
			//! Destructor
			virtual                 ~BaseLightningRenderer();
			//! Platform-dependent function called when initializing the lightning renderer.
			void                    RestoreDeviceObjects(crossplatform::RenderPlatform *r);
			//! Platform-dependent function called when uninitializing the lightning renderer.
			void                    InvalidateDeviceObjects();
			//! Once per-frame update. Do this before any rendering each frame.
			void                    PreRenderUpdate(crossplatform::GraphicsDeviceContext &deviceContext,float real_time);
			//! Main render call
			void                    Render( crossplatform::GraphicsDeviceContext& deviceContext,CloudKeyframer *cloudKeyframer
				                            ,crossplatform::Texture *depth_tex,vec4 depthViewportXYWH,const clouds::TransparencyAtmospherics &t
				                            ,float brightnessToUnity, clouds::BaseWeatherRenderer* weatherRenderer);
			//! Render debug information
			void                    RenderOverlay(crossplatform::GraphicsDeviceContext& deviceContext,Storm *);
			static void             EnsureEffectsAreBuilt(crossplatform::RenderPlatform *r);
			//! Platform-dependent function to reload the shaders - only use this for debug purposes.
			void                    RecompileShaders();
			LightningIllumination   GetLightningIllumination() const;
            void                    SetBaseSkyInterface(simul::sky::BaseSkyInterface* pSk);

		protected:
			void                    Recompile(crossplatform::GraphicsDeviceContext &deviceContext);

			crossplatform::RenderPlatform*  renderPlatform;
			simul::sky::BaseSkyInterface*   baseSkyInterface;
			crossplatform::Texture*         randomTexture;
			crossplatform::Effect*          effect;

            crossplatform::ShaderResource   _vertices;
            crossplatform::ShaderResource   _verticesRW;

            //! 0 is the main trunk, 1-4 hold branches 
			static const int maxLevels      = 5; 
            static const int maxBranches    = 12;

			crossplatform::StructuredBuffer<LightningVertex>			vertexBuffers[maxLevels];
			crossplatform::ConstantBuffer<LightningConstants>			lightningConstants;
			crossplatform::ConstantBuffer<LightningPerViewConstants>	lightningPerViewConstants;
			simul::clouds::LightningProperties                          props;

			float                                                       realTime;
			bool                                                        recompile_shaders;
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif