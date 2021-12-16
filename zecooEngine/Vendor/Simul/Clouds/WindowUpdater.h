#pragma once
#include "Simul/Clouds/Export.h"

#include "Simul/Clouds/CloudKeyframer.h"
#include "Simul/Clouds/Environment.h"
#include "Simul/Clouds/GpuCloudGenerator.h"
#include "Simul/Clouds/CloudRenderingOptions.h"
#include "Simul/Clouds/CloudWindow.h"
#include "Platform/Core/ProfilingInterface.h"
#include "Platform/CrossPlatform/Quaterniond.h"
#include "Platform/CrossPlatform/DeviceContext.h"
#include "Platform/CrossPlatform/BaseRenderer.h"
#include "Platform/CrossPlatform/SphereRenderer.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/gpu_cloud_constants.sl"
#include "Simul/Shaders/SL/cloud_constants.sl"
//#include "Simul/Shaders/SL/cloud_update_constants.sl"
#include "Simul/Shaders/SL/noise_constants.sl"
#include "Simul/Shaders/SL/sky_constants.sl"
#include "Platform/CrossPlatform/Effect.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

namespace simul
{
	namespace clouds
	{
		struct CloudLayer;
		struct DetailLevel
		{
			int amortize=1;
			int detail_width=0;
			int3 grid;
			int3 offset;
			int3 dispatch;
			int4 exclusion;
			uint8_t light_grid_z=1;
		};
		struct CompletionBlock
		{
			bool density_complete=false;
			bool lighting_complete=false;
		};
		struct CloudLayerState
		{
			CloudKeyframe cloudKeyframe;
			// offset from the window centre to the layer centre: this is for repeating layers. For others, we use origin to track an absolute world position.
			vec3 offsetKm;
			// This is for updating offsetKm. When the window's grid centre changes, we modify offsetKm.
			int2 WindowGridCentreTexel;
			int GridHeight;
			bool OverrideWind;
			crossplatform::Texture *map=nullptr;
			crossplatform::Texture* layer_texture = nullptr;
			VolumeMap volumes;
			CloudClass cloudClass;
			unsigned checksum;
			unsigned generation_number=0;
		};
		struct CloudVolumeTextures
		{
			void RestoreDeviceObjects(crossplatform::RenderPlatform *r);
			void InvalidateDeviceObjects();
			void Reset();
			crossplatform::RenderPlatform *renderPlatform=nullptr;
			crossplatform::Texture	*cloud_texture=nullptr;	// blended
			crossplatform::Texture	*light_texture=nullptr;
			int3 blockPos = { 0,0,0 };
			int lastBlockIndex=0;
			bool initialized=false;
		};
		SIMUL_CLOUDS_EXPORT_CLASS WindowUpdater
		{
			std::vector<CompletionBlock> completionBlocks;
			std::vector<int3> amortizeOffsets;
			std::vector<int3> amortizeOffsets2;	//2d offsets.
			//bool initializedCombinedTexture=false;
			int framenumber=0;
			//int merge_framenumber=0;
			int texture_cycle=-1;
			//unsigned last_true_checksum=0;
			/// A checksum to see if the interpolated cloud volume is out of date. This may fail every frame if time is moving continuously - that's ok.
			//unsigned last_interpolation_checksum=0;
			//unsigned last_subdivision_checksum=0;
			DetailLevel high, med, low;

			crossplatform::ShaderResource _cloudDensity;
			crossplatform::ShaderResource _cloudDensity2;
			crossplatform::ShaderResource _cloudMaskTexture;
			crossplatform::ShaderResource _vec4OutputTexture3d;
			crossplatform::ShaderResource _packedOutputTexture3d;
			crossplatform::ShaderResource _outputTexture3d;
			crossplatform::ShaderResource _cloudVolumes; 

			crossplatform::EffectTechnique* interpCloudsTechnique = nullptr;
			crossplatform::EffectTechnique* interpLightTechnique = nullptr;
			crossplatform::EffectPass* interpLightPass = nullptr;
			crossplatform::EffectTechnique* mixCloudsTechnique = nullptr;
			crossplatform::EffectTechnique* mixMapTechnique = nullptr;
			crossplatform::EffectTechnique* mixCirrusTechnique = nullptr;
			crossplatform::EffectTechnique* mixCumulusTechnique = nullptr;
			crossplatform::EffectTechnique* mixCumulonimbusTechnique = nullptr;
			crossplatform::EffectTechnique* mixCellNoiseTechnique = nullptr;
			
			crossplatform::RenderPlatform* renderPlatform=nullptr;
			
			crossplatform::ConstantBuffer<CloudConstants> cloudConstants;
			crossplatform::ConstantBuffer<CloudPerViewConstants> cloudPerViewConstants;
			crossplatform::ConstantBuffer<GpuCloudConstants> gpuCloudConstants;
			crossplatform::StructuredBuffer<CloudVolume_densityspace> cloudVolumes;
			crossplatform::Effect* effect=nullptr;
			std::vector<CloudLayerState> cloudLayerStates;
			unsigned long long frame_of_completion=0;
			unsigned long long frame_wait=0;
			int smooth_step=0;
			int smooth_mip=0;
			int mip_z=0;
		public:
			~WindowUpdater();
			void InitEffect(crossplatform::Effect* effect);
			void CompileEffect();
			void RestoreDeviceObjects(crossplatform::RenderPlatform *r);
			void InvalidateDeviceObjects();
			void RecompileShaders();
			void BlockDensity(crossplatform::DeviceContext& deviceContext
				, int3 blockPos
				, int3 blockFill
				, crossplatform::Texture* cloud_texture
				, crossplatform::Texture* light_valid
				, std::vector<CloudLayerState>& cloudLayers
				, CloudWindow& cloudWindow
				, crossplatform::Effect* effect
				);
			void BlockMips(crossplatform::DeviceContext& deviceContext
				, int3 blockPos
				, int3 blockFill
				, crossplatform::Texture* cloud_texture
				, crossplatform::Texture* light_valid
				, std::vector<CloudLayer*>& cloudLayers
				, CloudWindow& cloudWindow
				, crossplatform::Effect* effect
			);
			void BlockLight(crossplatform::DeviceContext& deviceContext
				, int3 blockPos
				, int3 blockFill
				, Environment* environment
				, std::vector<CloudLayer*>& cloudLayers
				, CloudWindow& cloudWindow
				, CloudRenderingOptions& cloudRenderingOptions
				, crossplatform::Effect* effect
				, crossplatform::Texture* cloud_texture
				, crossplatform::Texture* light_texture
				, crossplatform::Texture* light_valid
				,bool init
			);
			void UpdateNextCloudTexture(crossplatform::DeviceContext& deviceContext
				, Environment* environment
				, std::vector<CloudLayer*>& cloudLayers
				, CloudWindow& cloudWindow
				,float progress
				,int cycle
				, CloudRenderingOptions& cloudRenderingOptions
				, crossplatform::Effect* effect
				, CloudConstants& inCloudConstants
				, CloudPerViewConstants &inCloudPerViewConstants
				, GpuCloudConstants& inGpuCloudConstants
				, CloudVolumeTextures *cloudVolumeTextures
				, crossplatform::Texture* light_valid
				);
			void UpdateCombinedCloudTexture(crossplatform::DeviceContext& deviceContext,CloudVolumeTextures*
				, CloudRenderingOptions &cloudRenderingOptions
				, crossplatform::ConstantBuffer<CloudConstants>& cloudConstants
				, crossplatform::ConstantBuffer<CloudPerViewConstants>& cloudPerViewConstants
				, crossplatform::ConstantBuffer<GpuCloudConstants>& gpuCloudConstants);
			void InitCloudLayerStates(std::vector<CloudLayer*> &cloudLayers);
		protected:
			void UpdateCloudTexture(crossplatform::DeviceContext& deviceContext
				, Environment* environment
				, std::vector<CloudLayer*>& cloudLayers
				, CloudWindow& cloudWindow
				, float progress
				, int cycle
				, CloudRenderingOptions& cloudRenderingOptions
				, crossplatform::Effect* effect
				, CloudConstants& inCloudConstants
				, CloudPerViewConstants &inCloudPerViewConstants
				, GpuCloudConstants& inGpuCloudConstants
				, CloudVolumeTextures &cloudVolumeTextures
				, crossplatform::Texture* light_valid
				);
		};
		extern void MakeAmortizationOffsets3(std::vector<int3>& amortizeOffsets, int a);
		extern void MakeAmortizationOffsets2(std::vector<int3>& amortizeOffsets, int a);
	}
}

#ifdef _MSC_VER
	#pragma warning(pop)
#endif