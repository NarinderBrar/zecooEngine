#ifndef SIMUL_CLOUDS_CLOUDKEYFRAME_H
#define SIMUL_CLOUDS_CLOUDKEYFRAME_H
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

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif
#define TELL_CHECK										\
{														\
	int tell_2=is.tellg();								\
	if(tell_2!=-1&&(unsigned)tell_2>tell_2_required)	\
	{													\
		std::cerr<<"SkyKeyframer::Load - Position mismatch in seq file. Expected pos "<<tell_2_required<<" got pos "<<tell_2<<std::endl;	\
		is.seekg(tell_2_required);						\
	}													\
}
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
		enum CloudVolumeType
		{
			TRUESKY_CLOUD_UNKNOWN		=0
			,TRUESKY_CLOUD_CUMULONIMBUS
			,TRUESKY_CLOUD_SPHEROID
			,TRUESKY_CLOUD_CYLINDER
		};
		//! Custom volume. the transform matrix and scales are in km.
		struct CloudVolume
		{
			CloudVolumeType type;
		};
		struct CloudVolumeState
		{
			crossplatform::Quaterniond relative_centre;	// relative to keyframe origin.
			vec2 scales_lower_km;
			vec2 scales_upper_km;
			vec2 height_range_km;		// min and max.
			vec2 rake;
			float edge_lower;
			float edge_upper;
		};
		//! Custom volume.
		typedef std::map<sky::uid,CloudVolume> VolumeMap;

		//! A structure defining an area where there is rain or snow.
		//! If \em regional is false, it is everywhere.
		SIMUL_CLOUDS_EXPORT_STRUCT PrecipitationRegion
		{
			PrecipitationRegion() :regional(false),lockToClouds(true),radiusKm(50.f),rainEffectStrength(0.f),edge(0.1f), base_radial_km(6378)
			{
				relative_centre = crossplatform::Quaterniond(0.0, 0.0, 0.0, 1.0);
			}
			bool regional;								//!< If true, rain/snow only occurs within the specified region. If false, it occurs everywhere in the cloud keyframe.
			bool lockToClouds;							//!< If true, the movement of the region takes into account the wind's speed and heading.
			crossplatform::Quaterniond relative_centre;	//!< Origin on the globe for this Precipitation Region relative to keyframe origin.
			float radiusKm;								//!< The radius of the region.
			float rainEffectStrength;					//!< The visual strength of the virga effect when rendered.
			float edge;									//!< The edge between zero and one. Small numbers give a sharp edge to the region.
			float base_radial_km;						//!< The base altitude of this Precipitation Region from the centre of the planet.
		};
		//! The keyframe structure for clouds, used by simul::clouds::CloudKeyframer.
		SIMUL_CLOUDS_EXPORT_STRUCT CloudKeyframe : public sky::BaseKeyframe
		{
			float cloudiness;							//!< The density of the cloud layer at this time.
			float distribution_base_layer;				//!< Start of the transition from the cloud base to the upper cloud (0 to 1)
			float distribution_transition;				//!< The transition from the cloud base to the upper cloud (0 to 1)
			float upper_density;						//!< The proportion of cloud density retained in the upper layer, above the distribution_transition.
			float cloud_base_km;						//!< The base altitude of this cloud layer.
			float cloud_height_km;						//!< The height of the cloud layer, above its base altitude.
			float cloud_width_km;						//!< The width of the cloud layer in km, or the repeat-length.
			
			vec3 scale_km;								//!< For non-repeating clouds, the shape scale

			int octaves;								//!< The number of noise octaves used to generate the clouds; default 3.
			float persistence;							//!< The fractal persistence for generating the clouds, default 0.45.
			float fractal_amplitude;					//!< The strength of the fractal edge effect.

			float edge_sharpness;						//!< The sharpness to be applied in rendering a the boundary.
			float churn;								//!< The strength of the cloud edge churning effect - larger values for more turbulent clouds.
			float precipitation;						//!< The amount of rain/snow etc.
			float rain_to_snow;							//!< If zero, precipitation is interpreted as rain. If one, it is snow.
			float precipitation_base_km;				//!< The lowest altitude that the precipitation can occur; the highest altitude is dictated by cloud_base_km.

			float diffusivity;							//!< How much the edges of clouds should be diffused.
			float max_density_gm3;						//!< Grams per cubic metre. Default is 0.5
			float base_noise_factor;					//!< What proportion of noise is applied at the cloudbase, between 0 and 1.0.
			vec2 offsetKm;								//!< The calculated position offset of this keyframe due to the accumulation of wind motion (see \ref wind_speed and \ref wind_direction).
			float octaves_f;							//!< Only used for interpolation - changes will be ignored. See \ref octaves.
			float simulation;							//!< If 1.0, cloud volume is simulated. If 0.0, it is generated by fractal.
			float worley_noise;							//!< How much Worley (cell) noise to apply in cloud generation.
			float worley_scale;							//!< Scale of the Worley noise. Will be locked to an integer if clouds are wrapping.
			float edge_worley_noise;					//!< The strength of the fractal worley edge noise.

			crossplatform::Quaterniond origin;			//!< Origin on the globe for this layer.
			PrecipitationRegion precipitationRegion;	//!< Where rain/snow is found.
			std::map<sky::uid, CloudVolumeState> cloudVolumeStates; //!< States for cloud volumes listed in the keyframer.
			CloudKeyframe():sky::BaseKeyframe()
				,cloudiness(0.60f)
				,distribution_base_layer(0.13f)
				,distribution_transition(0.2f)
				,upper_density(0.85f)
				,cloud_base_km(3.f)
				,cloud_height_km(8.f)
				,cloud_width_km(120.f)
				,scale_km(200.f,200.f,10.f)
				,octaves(5)
				,persistence(0.1f)
				,fractal_amplitude(2.0f)
				,edge_sharpness(0.06f)
				,churn(1.f)
				,precipitation(0.f)
				,rain_to_snow(0.f)
				,precipitation_base_km(0.f)
				,diffusivity(0.45f)
				,max_density_gm3(0.5f)
				,base_noise_factor(0.75f)
				,offsetKm(0.f,0.f)
				,octaves_f(0.f)
				,simulation(0.0f)
				,worley_noise(0.125f)
				,worley_scale(3.0f)
				,edge_worley_noise(0.3f)
				,origin(0.70710679664085752,0,0,0.70710676573223719)
			{
			}
			~CloudKeyframe()
			{
			}
			virtual void Save(simul::sky::Output &os) const;
			virtual void Load(simul::sky::Input &is,simul::base::MemoryInterface *memoryInterface);
			virtual void SaveToText(simul::crossplatform::TextOutput &output) const;
			virtual void LoadFromText(simul::crossplatform::TextInput &input);
			/*! Return true if the keyframe has a float value with the given, case-insensitive, name; return false otherwise. 
			
			The properties are:	
				- cloudiness
				- distributionBaseLayer
				- distributionTransition
				- upperDensity
				- localDensity
				- persistence
				- cloudBase
				- cloudHeight
				- cloudWidth
				- cloudWidthKm
				- cloudWidthMetres
				- directLight
				- indirectLight
				- ambientLight
				- lightAsymmetry
				- precipitation
				- fractalAmplitude
				- edgeSharpness
				- churn
				- extinction
				- rainToSnow
				- precipitationBaseKm
				- precipitationWindEffect
				- precipitationWaver
				- diffusivity
				- cache_built
				- lightning
				- maxDensityGm3
				- baseNoiseFactor
				- offsetx
				- offsety
				- rainCentreXKm
				- rainCentreYKm
				- rainRadiusKm
				- rainEdge
				- rainEffectStrength
				- simulation
				- WorleyNoise
				- WorleyScale
				- EdgeWorleyNoise
				- scalekm
				- scalekm.x
				- scalekm.y
				- scalekm.z
				- EdgeWorleyScale
			*/
			virtual bool HasFloat(const char *name) const;
			//! Get a float with the given, case-insensitive, name
			virtual float GetFloat(const char *name) const;
			//! Set a float with the given, case-insensitive, name
			virtual void SetFloat(const char *name,float val);
			/*!Return true if the keyframe has an integer or true/false value with the given, case-insensitive, name; return false otherwise. 
			
				- Octaves
				- RegionalPrecipitation
				- LockRainToClouds
			*/
			virtual bool HasInt(const char *name) const;
			//! Get an int with the given, case-insensitive, name
			virtual int GetInt(const char *name) const;
			//! Set an int with the given, case-insensitive, name
			virtual void SetInt(const char *name,int val);
			//! Check for properties that, if altered in the keyframe, would require regeneration of
			//! the cloud volume:
			virtual unsigned GetPropertiesChecksum() const;
			
			// This function is used to access the true centre position of the precipitation region, depending on lockToClouds.
			// By default the function returns the absolute global position.
			crossplatform::Quaterniond GetPrecipitationCentre(bool relativeOrientation = false) const;
			
			/// From a relative orientation, get the absolute orientation.
			crossplatform::Quaterniond GetAbsoluteOrientation(const crossplatform::Quaterniond& rel) const;
			crossplatform::Quaterniond GetRelativeOrientation(const crossplatform::Quaterniond& abso) const;
		protected:
#ifndef DOXYGEN
			friend class CloudKeyframer;
#endif
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif