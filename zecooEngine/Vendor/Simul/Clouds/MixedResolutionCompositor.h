#pragma once
#include "Export.h"
#include "Platform/CrossPlatform/PixelFormat.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Platform/CrossPlatform/DeviceContext.h"
#include "Platform/CrossPlatform/Effect.h"
#include "Simul/Shaders/SL/mixed_resolution_constants.sl"
// NVCHANGE_BEGIN: TrueSky + VR MultiRes Support
#include "Simul/Shaders/SL/nv_multi_res_constants.sl"
// NVCHANGE_END: TrueSky + VR MultiRes Support
#include "Platform/Math/Orientation.h"
#include <set>
#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

namespace simul
{
	namespace crossplatform
	{
		class Texture;
		class BaseFramebuffer;
		class Effect;
		struct Viewport;
		class RenderPlatform;
	}
	namespace clouds
	{
		class TwoResFramebuffer;
		/// A class to render mixed-resolution depth buffers.
		class SIMUL_CLOUDS_EXPORT MixedResolutionCompositor
		{
		public:
			/// Constructor.
			MixedResolutionCompositor();
			/// Destructor.
			~MixedResolutionCompositor();

			//! Platform-dependent function called when initializing the Compositor.
			void RestoreDeviceObjects(crossplatform::RenderPlatform *renderPlatform);
			//! Platform-dependent function called when uninitializing the water Compositor.
			void InvalidateDeviceObjects();
			//! Platform-dependent function to reload the shaders - only use this for debug purposes.
			void RecompileShaders();

			void DownscaleDepth(crossplatform::GraphicsDeviceContext &deviceContext
				,crossplatform::Texture *depthTexture
				,const crossplatform::Viewport *depthViewports
				,const crossplatform::ViewStruct *viewStruct2
				,clouds::TwoResFramebuffer *fb
				,float max_dist_metres,float thresholdMetres,float DepthSamplingRange
												,bool two_step
												,bool cube_face=false
												,bool process_depth=true
												,float CloudDepthAlpha=0.1f);

			/// Avoid division by zero. Note: Be careful as 1 = 1m. so it will clip objects near the camera (10cm. works fine)
			float Minthreshold;

		protected:
			/// The render platform.
			crossplatform::RenderPlatform				*renderPlatform;
			/// The depth reverse effect.
			crossplatform::Effect						*effect;
			crossplatform::EffectTechnique				*minmaxTechnique;
			crossplatform::EffectTechnique				*stochasticTechnique;
			crossplatform::EffectTechnique				*stochasticTechniqueMSAA;
			crossplatform::EffectTechnique				*stochasticTwoEyesTechnique;

			crossplatform::EffectTechnique				*stochasticTwoEyesTechniqueMSAA;
			
			crossplatform::EffectTechnique				*directTechnique;
			
			/// The constant buffer for the shader effect.
			crossplatform::ConstantBuffer<MixedResolutionConstants>	mixedResolutionConstants;
			// NVCHANGE_BEGIN: TrueSky + VR MultiRes Support
			simul::crossplatform::ConstantBuffer<NvMultiResConstants2> nvMultiResConstants2;
			// NVCHANGE_END: TrueSky + VR MultiRes Support

			crossplatform::ShaderResource sourceTexture1;
			crossplatform::ShaderResource sourceTexture2;
			crossplatform::ShaderResource sourceTexture3;
			crossplatform::ShaderResource _previousCombinedTexture;
			crossplatform::ShaderResource _targetTexture;
			crossplatform::ShaderResource _targetTexture2;
			crossplatform::ShaderResource _targetTexture3;
			crossplatform::ShaderResource _nextUpdateTexture;
			crossplatform::ShaderResource _updateTexture;
			crossplatform::ShaderResource _sourceDepthTexture;
			crossplatform::ShaderResource _sourceMSDepthTexture;
			bool matrices_initialized;
		};

	}
}

#ifdef _MSC_VER
	#pragma warning(pop)
#endif
