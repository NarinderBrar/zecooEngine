#ifndef TWORESFRAMEBUFFER_H
#define TWORESFRAMEBUFFER_H
#include "Platform/CrossPlatform/BaseFramebuffer.h"
#include "Platform/CrossPlatform/AmortizationStruct.h"
#include "Platform/Math/Orientation.h"
#include "Export.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

namespace simul
{
	namespace crossplatform
	{
		class Texture;
		struct DeviceContext;
		class RenderPlatform;
		struct Viewport;
		struct ViewStruct;
	}
	namespace clouds
	{
		//! A framebuffer class for mixed-resolution rendering.
		class SIMUL_CLOUDS_EXPORT TwoResFramebuffer
		{
		public:
			TwoResFramebuffer();
			crossplatform::Texture *GetLowResFramebuffer(int index)
			{
				if(index<0||index>2)
					SIMUL_BREAK("Index out of range");
				return lowResFramebuffers[index];
			}
			virtual ~TwoResFramebuffer();
			bool IsLightpassTextureEmpty() const;
			void SetLightpassTextureEmpty(bool e);
			bool IsMainpassTextureEmpty() const;
			void SetMainpassTextureEmpty(bool e);
			crossplatform::Texture *GetLightpassTexture();
			crossplatform::Texture *GetVolumeTexture(int num);
			//! Platform-dependent function called when initializing the framebuffer.
			virtual void RestoreDeviceObjects(crossplatform::RenderPlatform *);
			//! Platform-dependent function called when uninitializing the water framebuffer.
			virtual void InvalidateDeviceObjects();
			void SetCubemapResolution(int c);
			int GetCubemapResolution() const
			{
				return CubemapResolution;
			}
			//! Assign the current frame's projection matrix for this buffer. Just for debugging.
			void SetProjection(const float *p);
			long long GetLastUpdateFramenumber() const
			{
				return last_framenumber;
			}
			//! Set the range of the cubemap that's in the current frustum.
			void SetCubeFrustumRange(int i,vec4 r);
			vec4 GetCubeFrustumRange(int i) const;
			/// Get the range in terms of the buffer size as integers.
			uint4 GetCubeIntegerFrustumRange(int i) const;
			uint4 GetLastRange(int i) const;
			void SetLastRange(int i,uint4 r);
			void SetDrawRange(int i,uint4 r);
			uint4 GetDrawRange(int i) const;
			/// Deactivate the depth buffer
			virtual void DeactivateDepth(crossplatform::GraphicsDeviceContext &);
			/// This must be called to ensure that the amortization struct is up to date.
			void CompleteFrame(long long framenumber);
			/// Debugging onscreen info:
			///
			/// \param [in,out]	deviceContext	Context for the device.
			/// \param	depthTexture			The main depth texture.
			/// \param	viewport			 	The viewport in use for the depth texture.
			/// \param	x0					 	The left edge of area to use for the debug display.
			/// \param	y0					 	The top of this debug display.
			/// \param	dx					 	The width of the display.
			/// \param	dy					 	The height of the display.
			void RenderDepthBuffers(crossplatform::GraphicsDeviceContext &deviceContext,crossplatform::Texture *depthTexture,const crossplatform::Viewport *viewport,int x0,int y0,int dx,int dy);

			/// Update the pixel offset for the specified view.
			void UpdatePixelOffset(const crossplatform::ViewStruct &viewStruct);
			/// Offset in pixels from top-left of the low-res view to top-left of the full-res.
			vec2								pixelOffset;
			/// Swap stochastic texture buffers.
			void Swap();
			void SwapUpdateTextures();
			/// Returns	the low-res depth texture.
			crossplatform::Texture				*GetStochasticDepthTexture(int idx=-1);
			crossplatform::Texture				*GetCombinedDepthTexture(int idx = -1);
			/// A texture that shows what texels are up to date. Where the value is zero, we should fill all the values.
			crossplatform::Texture				*GetUpdateTexture(int idx);
			crossplatform::PixelFormat GetDepthFormat() const;
			void SetDepthFormat(crossplatform::PixelFormat p);
			crossplatform::PixelFormat GetBufferFormat() const;
			void SetBufferFormat(crossplatform::PixelFormat p);
			int									final_octave;
			vec2								GetPixelOffset() const
			{
				return pixelOffset;
			}
			void								SetAmortization(int a)
			{
				amortizationStruct.setAmortization(a);
 			}
			void								SetFramesPerIncrement(int f)
			{
				amortizationStruct.framesPerIncrement=f;
			}
			int									GetAmortization() const
			{
				return amortizationStruct.getAmortization();
			}
			const crossplatform::AmortizationStruct			&GetAmortizationStruct() const
			{
				return amortizationStruct;
			}
		protected:
			int									CubemapResolution;
			vec4								cubeFrustumRange[6];
			uint4								cubeTargetRange[6];
			uint4								swapTargetRange[6];
			uint4								nextTargetRange[6];
			uint4								drawRange[6];
			mat4								proj;
			crossplatform::AmortizationStruct	amortizationStruct;
			int									volume_num;
			crossplatform::PixelFormat			depthFormat;
			crossplatform::PixelFormat			bufferFormat;
			simul::geometry::SimulOrientation	view_o;
			crossplatform::RenderPlatform		*renderPlatform;
			crossplatform::Texture				*nearFarTextures[4];
			crossplatform::Texture				*combinedDepthTextures[2];
			crossplatform::Texture				*updateTextures[2];
			crossplatform::Texture				*lightpassTexture;
			crossplatform::Texture				*volumeTextures[2];
			crossplatform::Texture				*lowResFramebuffers[3];
			bool								lightpassEmpty;
			bool								mainpassEmpty;
			long long							last_framenumber;
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif