#ifndef SIMUL_CLOUDS_BASEGPUCLOUDGENERATOR_H
#define SIMUL_CLOUDS_BASEGPUCLOUDGENERATOR_H
#include "Simul/Clouds/Export.h"
#include "Simul/Clouds/CellularCloudNode.h"
#include <map>
#include "Platform/Math/Vector3.h"
#include "Platform/CrossPlatform/Quaterniond.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/gpu_cloud_constants.sl"
#include "Platform/CrossPlatform/Effect.h"
#include "Platform/CrossPlatform/BaseFramebuffer.h"
#include "Simul/Sky/BaseKeyframer.h"
#include "Simul/Sky/BaseGpuSkyGenerator.h"
#include "Simul/Clouds/CloudKeyframe.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif
namespace simul
{
	namespace sky
	{
		class BaseSkyInterface;
		class OvercastCallback;
	}
	namespace crossplatform
	{
		class RenderPlatform;
		class Texture;
	}
	namespace clouds
	{
		SIMUL_CLOUDS_EXPORT_STRUCT Mask
		{
			crossplatform::Quaterniond centre;
			float radius;
			float thickness;
		};
		typedef std::map<int, Mask> MaskMap;
		enum LightingMethod
		{
			SIMPLE
			,DIFFUSION
			,HARMONIC_SECONDARY
		};
		//! By defining this typedef, we ensure that cloud texels are treated as 32-bit.
		typedef unsigned CloudTexelType;
		SIMUL_CLOUDS_EXPORT_STRUCT FillState
		{
			FillState()
				:dens_texel(0)
				,checksum(0)
			{}
			void Restart();
			int dens_texel;
			unsigned checksum;
		};
		SIMUL_CLOUDS_EXPORT_STRUCT KeyframeInUse:public FillState
		{
			KeyframeInUse();
			~KeyframeInUse();
			void Restart();
			void Clear();
			void Init(int w,int l,int d);
			int size;
			simul::base::MemoryInterface *memoryInterface;
			simul::math::Vector3		sun_direction;
			std::map<sky::uid,vec3>		moon_directions;
			simul::math::Vector3		sun_irradiance;
			simul::math::Vector3		moon_irradiance;

			LightingSetupStruct			lightingSetupStruct;

			bool						texture_complete;
			float						interp;
            const CloudKeyframe &GetKeyframe() const
            {
                return keyframe;
            }
            CloudKeyframe &GetKeyframe()
            {
                return keyframe;
            }
		private:
			CloudKeyframe	keyframe;
		};
		struct CloudSimulationParameters
		{
			vec3 upperWindKms;
			float vorticityConfinement;
			vec3 lowerWindKms;
			float viscosity;
		};
		SIMUL_CLOUDS_EXPORT_STRUCT GpuCloudsParameters
		{
			int3	density_grid;
			int3	light_grid;
			float	humidity;
			float	baseLayer;
			float	transition;
			float	upperDensity;
			float	diffusivity;
			float	time;
			int		octaves;
			float	persistence;
			int		noiseSize;
			mat4	Matrix4x4LightToDensityTexcoords;
			mat4	Matrix4x4AmbientToDensityTexcoords;
			mat4	Matrix4x4DensityToLightTransform;
			vec3	cornerM;
			vec3	densityVolumeScalesM;
			vec4	lightspace_extinctions;
			bool wrap_light_tex[2];
			const simul::clouds::VolumeMap *cloudVolumes;
			const float *noise_src_ptr;
			int		noise_generation_number;
			float	texture_fill_progress;
			bool	wrap_horizontally;
			unsigned checksum;
			vec3	directionToLight;
			float	seaLevelTemperatureK;
			float	numWorleyOctaves;
			float	worleyNoiseStrength;
			float	worleyNoiseScale;
			int		mips;
		};
		//! Base class for generating cloud volumes on the GPU.
		SIMUL_CLOUDS_EXPORT_CLASS BaseGpuCloudGenerator
		{
		public:
			BaseGpuCloudGenerator();
			virtual ~BaseGpuCloudGenerator(); // RDE: added virtual
			void SetLightingMethod(LightingMethod h)
			{
				lightingMethod=h;
			}
			META_BeginProperties
				META_Property(bool	,Enabled,"Whether this GPU cloud generator should be used to calculate the clouds.")
			META_EndProperties
			void ResetChecksums();
			void SetCloudPlacementTexture(int id,crossplatform::Texture *t,vec2 pos_km,vec2 ext_km);
			virtual void SetTextureCycle(int c);
			virtual void RestoreDeviceObjects(crossplatform::RenderPlatform *);
			virtual void InvalidateDeviceObjects();
			virtual void RecompileShaders();
			//! Return true if the derived class can relight clouds using the GPU.
			virtual bool CanPerformGPULighting() const
			{
				return Enabled&&renderPlatform!=NULL;
			}
			void Update(crossplatform::DeviceContext &deviceContext,int cycled_index,const clouds::GpuCloudsParameters &params
				,crossplatform::Texture *finalTexture[3],FillState fillStates[3],CloudClass cloudClass);
			//! Get the size, in floats, needed to allocate a texture of the size specified in grid[3].
			//! This depends on what float renderbuffer formats are supported.
			virtual int GetDensityGridsize(const int *grid);
			//! Create an API-specific noise texture, and return its pointer/identifier etc. as a void pointer.
			virtual crossplatform::Texture* Make3DNoiseTexture(crossplatform::DeviceContext &deviceContext,int noise_size,
											const float  *noise_src_ptr,int generation_number);
			//! Fill a 3D grid with floating point cloud densities, and return an API-specific
			//! identifier to the texture that it's stored in.
			//! We pass the start texel and the number of texels to fill, which should both be a whole multiple of grid[0]
			virtual void FillDensityGrid(crossplatform::DeviceContext &deviceContext
				, int index, const GpuCloudsParameters &params
				, int start_texel
				, int texels
				, crossplatform::Texture *finalTexture
				, CloudClass cloudClass);
			//! For onscreen debug
			void RenderSimulationTextures(crossplatform::GraphicsDeviceContext &context,int x0,int y0,int width,int height);
			virtual float GetOpticalPathLength(int /*cycled_index*/,const float * /*pos*/,const float * /*dir*/,const float * /*scales*/)
			{
				return 0.f;
			}
			void SetWorleyTexture(crossplatform::Texture *t)
			{
				worleyTexture3D=t;
			}
		protected:
			crossplatform::RenderPlatform		*renderPlatform;
			
			crossplatform::Effect*				effect;
			crossplatform::EffectTechnique*		densityComputeTechnique; 
			crossplatform::EffectTechnique*		cumulusTechnique=nullptr; 
			crossplatform::EffectTechnique*		compatibility41aTechnique=nullptr;
			crossplatform::EffectTechnique*		cellNoise = nullptr;
			crossplatform::EffectTechnique*		jamesWIP = nullptr;
			crossplatform::EffectTechnique*		maskTechnique;
			crossplatform::EffectTechnique*		secondaryHarmonicTechnique;

			crossplatform::Texture				*volume_noise_tex;
			crossplatform::Texture				*worleyTexture3D;
			// Simulation textures:The basic quantities necessary to simulate clouds are
			// velocity, u=(u, v, w), air pressure, p, temperature T, water
			// vapor, qv, and condensed cloud water, qc. These water
			// content variables are mixing ratios: quantity per unit air mass
			crossplatform::Texture				*simulationVelocityDensity1;
			crossplatform::Texture				*simulationVelocityDensity2;
			crossplatform::Texture				*velocityDivergence;
			crossplatform::Texture				*simulationPressure1;
			crossplatform::Texture				*simulationPressure2;
			crossplatform::Texture				*simulationProperties1;
			crossplatform::Texture				*simulationProperties2;
			crossplatform::Texture				*simulationVorticity;
			crossplatform::Texture				*simulationVorticityField;

			crossplatform::ConstantBuffer<GpuCloudConstants>	gpuCloudConstants;
			
			crossplatform::SamplerState			*m_pWwcSamplerState;
			crossplatform::SamplerState			*m_pWccSamplerState;
			crossplatform::SamplerState			*m_pCwcSamplerState;
			crossplatform::SamplerState			*m_pCccSamplerState;
			LightingMethod lightingMethod;
			int texture_cycle;
			int noiseSize;
			int last_generation_number;
			static void SetGpuCloudConstants(GpuCloudConstants &g,const clouds::GpuCloudsParameters &params,float y_start,float y_range);
			struct CloudPlacementTexture
			{
				crossplatform::Texture *texture;
				vec2 pos_km,ext_km;
			};
			std::map<int,CloudPlacementTexture> cloudPlacementTextures;
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif