#ifndef SIMUL_CROSSPLATFORM_HDRRENDERER_H
#define SIMUL_CROSSPLATFORM_HDRRENDERER_H
#include "Platform/Core/PropertyMacros.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Platform/Shaders/SL/hdr_constants.sl"
#include "Platform/Shaders/SL/image_constants.sl"
#include "Platform/CrossPlatform/Export.h"
#include "Platform/CrossPlatform/RenderPlatform.h"
#include "Platform/CrossPlatform/Effect.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif
namespace simul
{
	namespace crossplatform
	{
		class SIMUL_CROSSPLATFORM_EXPORT HdrRenderer
		{
		public:
			HdrRenderer();
			virtual ~HdrRenderer();
			META_BeginProperties
				META_Property(bool,Glow,"Whether to apply a glow effect")
			META_EndProperties
			void SetBufferSize(int w,int h);
			//! Platform-dependent function called when initializing the HDR renderer.
			void RestoreDeviceObjects(crossplatform::RenderPlatform *r);
			//! Platform-dependent function called when uninitializing the HDR renderer.
			void InvalidateDeviceObjects();
			//! Render: write the given texture to screen using the HDR rendering shaders
			void Render(GraphicsDeviceContext &deviceContext,crossplatform::Texture *texture,float Exposure,float Gamma,float offsetX);
			void Render(GraphicsDeviceContext &deviceContext,crossplatform::Texture *texture,float Exposure,float Gamma);
			void RenderInfraRed(GraphicsDeviceContext &deviceContext,crossplatform::Texture *texture,vec3 infrared_integration_factors,float Exposure,float Gamma);
			void RenderWithOculusCorrection(GraphicsDeviceContext &deviceContext,crossplatform::Texture *texture,float Exposure,float Gamma,float offsetX);
			//! Create the glow texture that will be overlaid due to strong lights.
			void RenderGlowTexture(GraphicsDeviceContext &deviceContext,crossplatform::Texture *texture);
			//! Draw the debug textures
			void RenderDebug(GraphicsDeviceContext &deviceContext, int x0, int y0, int w, int h);
			static void EnsureEffectsAreBuilt(crossplatform::RenderPlatform *r);
			void RecompileShaders();
			crossplatform::Texture *GetBlurTexture();
		protected:
			crossplatform::RenderPlatform		*renderPlatform;
			crossplatform::Texture				*brightpassTextures[4];
			simul::crossplatform::Texture		*glowTextures[4];
			simul::crossplatform::Texture		*blurTexture;
			int Width,Height;
			void DoGaussian(crossplatform::DeviceContext &deviceContext,crossplatform::Texture *brightpassTexture,crossplatform::Texture *targetTexture);
		
			//! The HDR tonemapping hlsl effect used to render the hdr buffer to an ldr screen.
			crossplatform::Effect*				hdr_effect;
			crossplatform::EffectTechnique*		exposureGammaTechnique;
			crossplatform::EffectTechnique*		glowExposureGammaTechnique;
			crossplatform::EffectTechnique*		warpExposureGamma;
			crossplatform::EffectTechnique*		warpGlowExposureGamma;
			
			crossplatform::EffectTechnique*		glowTechnique;
			
			crossplatform::Effect*				m_pGaussianEffect;
			crossplatform::EffectTechnique*		gaussianRowTechnique;
			crossplatform::EffectTechnique*		gaussianColTechnique;

			crossplatform::ConstantBuffer<HdrConstants>			hdrConstants;
			crossplatform::ConstantBuffer<ImageConstants>		imageConstants;
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif