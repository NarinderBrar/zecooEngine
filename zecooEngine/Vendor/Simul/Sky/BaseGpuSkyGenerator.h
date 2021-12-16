#ifndef SIMUL_CLOUDS_BASEGPUSKYGENERATOR_H
#define SIMUL_CLOUDS_BASEGPUSKYGENERATOR_H
#include "Simul/Sky/Export.h"
#include "Simul/Sky/Float4.h"
#include "Simul/Sky/SkyInterface.h"
#include "Platform/Core/PropertyMacros.h"
#include "Simul/Sky/Texture3D4.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/gpu_sky_constants.sl"
#include "Platform/CrossPlatform/Effect.h"
#include <vector>

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif

namespace simul
{
	namespace crossplatform
	{
		class RenderPlatform;
		class Texture;
	}
	namespace sky
	{
		class AtmosphericScatteringInterface;
		SIMUL_SKY_EXPORT_STRUCT GpuTexGenStats
		{
			int numElevations;
			int numDistances;
			int numAltitudes;
			float min_alt_km;
			float max_alt_km;
			float max_distance_km;
			int fill_up_to_texels;
			unsigned keyframe_checksum;
		};
		SIMUL_SKY_EXPORT_STRUCT GpuMoon
		{
			vec3 dir;
			float radius_radians;
			vec3 irradiance_factor;
		};
		SIMUL_SKY_EXPORT_STRUCT GpuPhysicalSkyParameters
		{
			simul::sky::float4 sun_irradiance;
			simul::sky::float4 starlight;
			vec3 dir_to_sun;
			GpuMoon moons[10];
			int num_moons;
			simul::sky::HazeStruct hazeStruct;
			simul::sky::float4 ozone;
			float sun_start_alt_km;
			float sun_radius_radians;
		};
		SIMUL_SKY_EXPORT_STRUCT GpuColourTextureSpec
		{
			// up, horizon, down
			vec3 horizon;
			vec3 zenith;
			float interp;
		};
		SIMUL_SKY_EXPORT_STRUCT GpuSkyParameters:public GpuTexGenStats
		{
			float blendToColours;
			float brightnessPower;
			GpuPhysicalSkyParameters physical;
			GpuColourTextureSpec colour;
		};
		SIMUL_SKY_EXPORT_STRUCT GpuSkyAtmosphereParameters
		{
			const simul::sky::float4 *density_table;
			const simul::sky::float4 *optical_table;
			const simul::sky::float4 *blackbody_table;
			int table_size;
			float maxDensityAltKm;
			unsigned tables_checksum;
			float planet_radius_km;
		};
		SIMUL_SKY_EXPORT_STRUCT GpuSkyInfraredParameters 
		{
			//bool InfraRed;
			float emissivity;
			float seaLevelTemperatureK;
		};
		struct HazeStruct;
		SIMUL_SKY_EXPORT_CLASS BaseGpuSkyGenerator
		{
		protected:
			bool Enabled;
		public:
			BaseGpuSkyGenerator();
			virtual ~BaseGpuSkyGenerator();
			void Reset();
			/// Whether this GPU sky generator should be used to calculate the sky.")
			bool GetEnabled() const
			{
				return Enabled&(CanPerformGPUGeneration());
			}
			void SetEnabled(bool v)
			{
				Enabled=v;
			}
			//! Platform-dependent function called when initializing the Sky Generator.
			virtual void RestoreDeviceObjects(crossplatform::RenderPlatform *);
			//! Platform-dependent function called when uninitializing the Sky Generator.
			virtual void InvalidateDeviceObjects();
			//! Platform-dependent function to reload the shaders - only use this for debug purposes.
			virtual void RecompileShaders();
			// If we want the generator to put the data directly into 3d textures:
			virtual void SetDirectTargets(crossplatform::Texture **loss,crossplatform::Texture **insc,crossplatform::Texture **skyl,crossplatform::StructuredBuffer<vec4, crossplatform::BufferUsageHint::ONCE> **brt,crossplatform::Texture *light_t)
			{
				for(int i=0;i<3;i++)
				{
					if(loss)
						finalLoss[i]=loss[i];
					else
						finalLoss[i]=NULL;
					if(insc)
						finalInsc[i]=insc[i];
					else
						finalInsc[i]=NULL;
					if(skyl)
						finalSkyl[i]=skyl[i];
					else
						finalSkyl[i]=NULL;
					if(brt&&brt[i])
						skyBrightnessSB[i]=brt[i];
					light_table=light_t;
				}
			}
			//! Return true if the derived class can make sky tables using the GPU.
			virtual bool CanPerformGPUGeneration() const
			{
				return renderPlatform!=NULL;
			}
			float GetBrightness(int cycled_index) const;
			virtual void MakeLossAndInscatterTextures(
				crossplatform::DeviceContext &deviceContext,float4 wavelengthsNm,int cycled_index,
				simul::sky::AtmosphericScatteringInterface *skyInterface
				,const GpuSkyParameters &gpuSkyParameters
				,const GpuSkyAtmosphereParameters &gpuSkyAtmosphereParameters
				, const GpuSkyInfraredParameters &gpuSkyInfraredParameters);
			void MakeLossAndInscatterTextures(
				crossplatform::DeviceContext &deviceContext,
				sky::float4 wavelengthsNm
				, simul::sky::AtmosphericScatteringInterface *skyInterface
				, const sky::GpuSkyParameters &p
				, const sky::GpuSkyAtmosphereParameters &a
				, const sky::GpuSkyInfraredParameters &ir
				, int &texIndex
				, crossplatform::Texture *loss
				, crossplatform::Texture *insc
				, crossplatform::Texture *skyl
				, crossplatform::StructuredBuffer<vec4, crossplatform::BufferUsageHint::ONCE> *skyBrightness
				, int cycled_index);
			crossplatform::Texture *GetOpticalDepthTexture()
			{
				return optd_tex;
			}
			crossplatform::Texture *GetDensityTexture()
			{
				return dens_tex;
			}
		protected:
			void SetGpuSkyConstants(GpuSkyConstants &c,const GpuSkyParameters &p,const GpuSkyAtmosphereParameters &a,const GpuSkyInfraredParameters &i);
			unsigned keyframe_checksums[3];
			int fadeTexIndex[3];
			crossplatform::RenderPlatform		*renderPlatform;
			crossplatform::Effect				*effect;
			crossplatform::Texture				*finalLoss[3];
			crossplatform::Texture				*finalInsc[3];
			crossplatform::Texture				*finalSkyl[3];
			crossplatform::Texture				*light_table;
			crossplatform::EffectTechnique		*lossComputeTechnique;
			crossplatform::EffectTechnique		*inscComputeTechnique;
			crossplatform::EffectTechnique		*subdivisionBrightnessTechnique;
			crossplatform::EffectTechnique		*skylComputeTechnique;
			crossplatform::EffectTechnique		*lightComputeTechnique;
			crossplatform::ShaderResource		_loss_texture;		
			crossplatform::ShaderResource		_optical_depth_texture;	
			crossplatform::ShaderResource		_insc_texture;		
			crossplatform::ShaderResource		_density_texture;		
			crossplatform::ShaderResource		_targetTexture;
			crossplatform::ShaderResource		_blackbody_texture;
			crossplatform::ShaderResource		_colourTexture;
			crossplatform::ShaderResource		_skyBrightnessRW;
			//crossplatform::ShaderResource		_skyBrightness;
			
			crossplatform::ConstantBuffer<GpuSkyConstants>	gpuSkyConstants;
			crossplatform::StructuredBuffer<vec4, crossplatform::BufferUsageHint::ONCE> *skyBrightnessSB[3];
			std::vector<vec4> skyBrightness[3];
			crossplatform::Texture				*dens_tex,*optd_tex,*bb_tex;
			
			crossplatform::Texture				*inscColours[2];

			crossplatform::Texture				*inscColourTexture;
			crossplatform::SamplerState			*cmcSamplerState;
			unsigned							tables_checksum;
		};
	}
}

#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#endif
