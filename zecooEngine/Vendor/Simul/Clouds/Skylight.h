#pragma once
#include "Platform/CrossPlatform/BaseFramebuffer.h"
#include "Platform/CrossPlatform/SphericalHarmonics.h"
#include "Simul/Clouds/Export.h"

namespace simul
{
	namespace clouds
	{
		class BaseWeatherRenderer;
		/// A class to capture light from the sky in real time for diffuse and specular lighting.
		class Skylight
		{
			static int count;
			crossplatform::RenderPlatform *renderPlatform;
			crossplatform::SphericalHarmonics sphericalHarmonics;
		public:
			SIMUL_CLOUDS_EXPORT Skylight();
			SIMUL_CLOUDS_EXPORT ~Skylight();
			/// Fill in the texture.
			void SIMUL_CLOUDS_EXPORT Render(crossplatform::GraphicsDeviceContext &parentDeviceContext,BaseWeatherRenderer *baseWeatherRenderer
				,int cube_id,crossplatform::BaseFramebuffer *fb
				,int faceIndex,const mat4 &engineToSimulMatrix4x4,float blend, float exposure=1.0f, float gamma=1.0f);
			/// Update the skylight per-frame.
			void SIMUL_CLOUDS_EXPORT Update(crossplatform::GraphicsDeviceContext &deviceContext, BaseWeatherRenderer *weatherRenderer);
			/// Initialize GPU objects.
			void SIMUL_CLOUDS_EXPORT RestoreDeviceObjects(crossplatform::RenderPlatform *r);
			/// Free the GPU objects.
			void SIMUL_CLOUDS_EXPORT InvalidateDeviceObjects();
			/// Recompile the shaders for this class.
			void SIMUL_CLOUDS_EXPORT RecompileShaders();
			/// Show debug displays onscreen.
			void SIMUL_CLOUDS_EXPORT ShowDebug(crossplatform::GraphicsDeviceContext &deviceContext, int X, int Y, int w);
			///
			bool GetSphericalHarmonics(crossplatform::DeviceContext &deviceContext,float *targetShValues);
			/// 

			bool Probe(crossplatform::DeviceContext &deviceContext
				, int mip_size
				, int face_index
				, uint2 pos
				, uint2 size
				, vec4 *targetValuesFloat4);
			crossplatform::BaseFramebuffer *frameBuffer;
			int size;
			int mips;
			int view_id;
			crossplatform::PixelFormat format;

			int faceIndex;
			int currentMip;
			int bands;
			int last_update;
			int updateFrequency;
			bool initialized;
			bool fully_initialized;
			bool draw_clouds;
			float blend;
			float exposure;
			float gamma;
			vec3 ground_colour;
			mat4 engineToSimulMatrix4x4;
			int amortization;
			bool allFaces;
			bool allMips;
		};
	}
}