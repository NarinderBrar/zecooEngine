#ifndef SIMUL_SKY_INTERFACE_H
#define SIMUL_SKY_INTERFACE_H

#include "Simul/Sky/Export.h"
#include "Platform/Core/PropertyMacros.h"
#include "Simul/Base/Variant.h"
#include "Simul/Sky/Float4.h"
#include "Simul/Sky/Texture3D4.h"
#include "Simul/Sky/BaseKeyframer.h"
#include <map>

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif

namespace simul
{
	namespace sky
	{
		mat4 WorldspaceToLightspaceMatrix(vec3 lightDir);
		mat4 WorldspaceToLightspaceMatrix(vec3 lightDir,vec3 origin_w,float zextent,float rextent);
		//! A data structure to contain haze properties in the atmosphere at any given moment.
		SIMUL_SKY_EXPORT_STRUCT HazeStruct
		{
			HazeStruct()
				:haze(0)
				,haze_base_height_km(0)
				,haze_scale_height_km(0)
				,eccentricity(0)
				,mie(0,0,0,0)
				,rayleigh(0,0,0,0)
				,BaseOzone(0,0,0,0)
				,OzoneStrength(0.f)
				,SeaLevelTemperatureK(288.15f)
			{}
			HazeStruct(float h,float b,float s,float e,const float4 &m,const float4 &r,const float4 &o,float oz,float t_k)
				:haze(h)
				,haze_base_height_km(b)
				,haze_scale_height_km(s)
				,eccentricity(e)
				,mie(m)
				,rayleigh(r)
				,BaseOzone(o)
				,OzoneStrength(oz)
				,SeaLevelTemperatureK(t_k)
			{}
			float haze;
			float haze_base_height_km;
			float haze_scale_height_km;
			float eccentricity;
			float4 mie;
			float4 rayleigh;
			float4 BaseOzone;
			float OzoneStrength;
			float SeaLevelTemperatureK;
		};
		SIMUL_SKY_EXPORT_STRUCT OvercastStruct
		{
			OvercastStruct()
				:overcast(0.0f)
				,overcast_base_km(0.0f)
				,overcast_scale_km(1.0f)
			{}
			OvercastStruct(float o,float ob,float os)
				:overcast(o)
				,overcast_base_km(ob)
				,overcast_scale_km(os)
			{}
			float overcast;
			float overcast_base_km;
			float overcast_scale_km;
		};
		// The per-frame cache of lighting values, this is altitude-dependent and calculated in Update().
		SIMUL_SKY_EXPORT_STRUCT LightingState
		{
			LightingState()
				:DirectionToSun(0,0,0)
				{}
			vec3 DirectionToSun;
			std::map<uid,vec3> MoonDirections;
			void Reset()
			{
				MoonDirections.clear();
				DirectionToSun.x = DirectionToSun.y = DirectionToSun.z = 0;
			}
		};
		enum SkyKeyframeOverrides
		{
			SKY_KF_OVERRIDE_NONE=0
			,SKY_KF_OVERRIDE_SUN
			,SKY_KF_OVERRIDE_MOON
		};
		inline SkyKeyframeOverrides operator|(SkyKeyframeOverrides a, SkyKeyframeOverrides b)
		{
			return static_cast<SkyKeyframeOverrides>(static_cast<int64_t>(a) | static_cast<int64_t>(b));
		}
		inline SkyKeyframeOverrides operator&(SkyKeyframeOverrides a, SkyKeyframeOverrides b)
		{
			return static_cast<SkyKeyframeOverrides>(static_cast<int64_t>(a) & static_cast<int64_t>(b));
		}
		/// The keyframe structure for simul::sky::SkyKeyframer.

		///	The properties of any sky keyframe can be changed at any time. However, only HazeEccentricity
		/// can be changed for an in-use keyframe without triggering a recalculation of the colour tables.
		SIMUL_SKY_EXPORT_STRUCT SkyKeyframe : public BaseKeyframe
		{
			float	daytime;				//!< The time this keyframe represents, in a floating-point number of days from the starting midnight of the sequence. If simul::sky::SkyKeyframer::SetLinkKeyframeTimeAndDaytime is true, this is not used.
			
			mutable float4	Mie;			//!< The Mie scattering coefficients (x=red,y=green,z=blue). Mutable because of AutoMie.
			float	Haze;					//!< The amount of haze, mist or fog.
			float	HazeBaseKm;				//!< The base altitude, above which haze starts to decrease in density.
			float	HazeScaleKm;			//!< The vertical scale over which haze reduces with altitude.
			float	HazeEccentricity;		//!< The anisotropy of Mie scattering. Can be changed without recalculating tables.

			float GroundFog;				//!< Thick ground fog
			float FogCeilingKm;				//!< Upper limit of ground fog


			//mutable float	SunAzimuth;		//!< Horizontal angle to the sun. This will be overridden if sun position is automatic.
			//mutable float	SunElevation;	//!< Angle of the sun above the horizon. This will be overridden if sun position is automatic.
	
			float	SeaLevelTemperatureK;	//!< Temperature at sealevel, in kelvins - mainly relevant to infra-red.
			float	StoreAsColours;			//!< If true, the keyframe is stored as a colour table, rather than generating its colours from its properties.
			vec3	Horizon;
			vec3	Zenith;
			vec3	Sunlight;
			vec3	HazeColour;

			bool	AutoMie;				//!< Whether to recalculate the Mie coefficients based on the haze at this keyframe.
			
			bool	automaticSunPosition;	//!< Whether to calculate the sun position from the date and time.
		
			SkyKeyframe(base::MemoryInterface *m=NULL);
			virtual ~SkyKeyframe(){};
			void SetMemoryInterface(base::MemoryInterface *m);
			virtual void Save(Output &os) const;
			virtual void Load(Input &is,simul::base::MemoryInterface *memoryInterface);
			virtual void SaveToText(crossplatform::TextOutput &os) const;
			virtual void LoadFromText(crossplatform::TextInput &is);
			void ApplyLimits();
			virtual bool HasFloat(const char *name) const;
			virtual float GetFloat(const char *name) const;
			virtual void SetFloat(const char *name,float val);
			virtual bool HasInt(const char *name) const;
			virtual int GetInt(const char *name) const;
			virtual void SetInt(const char *name,int val);
			virtual bool Has(const char *) const;
			virtual void Set(const char *,const base::Variant *);
			virtual base::Variant Get(const char *,const base::Variant *);
			//! Checksum including values not needed for GPU recalculation.
			unsigned GetTransientChecksum() const;
			virtual unsigned GetPropertiesChecksum() const;
			virtual float4 GetColour(const char *name) const;
			virtual void SetColour(const char *name,float4 val);
		};
		/// There are three keyframes in use.
		/// Two are complete, one is in construction.
		/// The keyframe values are interpolated from the true, control keyframes.
		SIMUL_SKY_EXPORT_STRUCT KeyframeInUseState
		{
			KeyframeInUseState()
				:checksum(0)
				,sunStartAltKm(0)
				,keyframe()
			{
			};
			void Reset()
			{
				lightingState.Reset();
				keyframe.SetChecksum(0);
				checksum=0;
			}
			void SetMemoryInterface(simul::base::MemoryInterface *m)
			{
				keyframe.SetMemoryInterface(m);
			}
            const SkyKeyframe &GetKeyframe() const
            {
                return keyframe;
            }
            SkyKeyframe &GetKeyframe()
            {
                return keyframe;
            }
			LightingState lightingState;
			unsigned checksum;
			float sunStartAltKm;
        private:
			SkyKeyframe keyframe;
		};
		//! Properties of the Earth's shadow on the atmosphere and other objects.
		SIMUL_SKY_EXPORT_STRUCT EarthShadow
		{
			bool enable;					//!< If true, the Earth's shadow should be visible above the horizon.
			vec3 normal;					//!< normal is the direction normal to the sunlight direction
			float radius_on_cylinder;		//!< radius_on_cylinder is the distance from the Earth's centre to the viewpoint on the plane of the cylinder
			float illumination_altitude;	//!< What altitude (in km) is the lowest that sunlight reaches, directly above/below the viewer.
			float planet_radius;			//!< In km.
			float terminator_distance_km;	//!< The shortest distance from the viewer TO the terminator, which will be in the direction TO the sun.
		};
		SIMUL_SKY_EXPORT_STRUCT Orbit
		{
			double LongitudeOfAscendingNode=125.1228;
			double LongitudeOfAscendingNodeRate=-0.0529538083;
			double Inclination=5.1454;
			double ArgumentOfPericentre=318.0634;
			double ArgumentOfPericentreRate=0.1643573223;
			double MeanDistance=60.2666;
			double Eccentricity=0.054900;
			double MeanAnomaly=115.3654;
			double MeanAnomalyRate=13.0649929509;
		};
		SIMUL_SKY_EXPORT_STRUCT Moon
		{
			Orbit orbit;
			float radiusArcMinutes=16.f;
			std::string textureFilename;
			vec3 colour={1.f,1.f,1.f};
			float albedo=0.136f;
			void SaveToText(crossplatform::TextOutput &os,uid u) const;
			void LoadFromText(crossplatform::TextInput &is);
		};
		//! An abstract interface class for a sky that is calculated using sidereal data.
		SIMUL_SKY_EXPORT_CLASS SiderealSkyInterface
		{
		public:
			//! Get a 4x4 transformation matrix to transform sidereal coordinates into azimuth and elevation.
			virtual const float *SiderealToEarthMatrix(double epoch_time,float latitude,float longitude) const=0;
			virtual void GetSunPosition(float &azimuth,float &elevation,double epoch_time,float latitude,float longitude) const=0;
			virtual void GetMoonPosition(float &azimuth,float &elevation,double epoch_time,float latitude,float longitude,const Orbit &orbit) const=0;
		
			virtual double GetDawn(double epoch_time,float latitude,float longitude) const=0;
			virtual double GetSunset(double epoch_time,float latitude,float longitude) const=0;
		};
		//! An abstract interface class for skies that can calculate optical lengths.
		SIMUL_SKY_EXPORT_CLASS OpticalLengthInterface
		{
		public:
			// OpticalLengthInterface
			virtual float GetOpticalLength(float Elevation,float h_km,float distance_to_edge) const=0;
			//! Get the effective optical length (equivalent length at sea level density) of a ray
			//! cast from altitude \a h_km at angle \a Elevation above the horizon.
			virtual float GetOpticalLength(float Elevation,float h_km) const=0;
			//! Get the effective optical length of haze of a ray cast from altitude \a
			//! h_km at angle \a Elevation above the horizon.
			virtual float GetHazeFactorOpticalLength(float Elevation,float h_km,HazeStruct hazeStruct) const=0;
			//! Get the effective optical length of ozone of a ray cast from altitude \a
			//! h_km at angle \a Elevation above the horizon.
			virtual float GetOzoneOpticalLength(float Elevation,float h_km) const=0;
		};
		//! An abstract interface class for skies.
		SIMUL_SKY_EXPORT_CLASS BaseSkyInterface
		{
			VIRTUAL_GetAndSet(float	,Time					,"Time value, usually as a daytime from 0 to 1. Where timezones are used, Time is local.")
			VIRTUAL_Get(float4			,ColourWavelengthsNm	,"Wavelengths in nanometres, xyz = red, green, blue. These values are used to calculate the Mie coefficients is MieWavelengthExponent is non-zero, and the Rayleigh coefficients if CalcAutomaticRayleighCoefficients is called.")
			VIRTUAL_Get(float			,MieEccentricity		,"Eccentricity of Mie (haze) scattering in the sky.")
			//! Get the direction to the current strongest light source - could be the sun or the moon.
			virtual vec3 GetDirectionToLight(double t,float altitude_km) const=0;
			//! Get the direction to the current strongest light source - could be the sun or the moon.
			virtual vec3 GetDirectionToLight(float altitude_km) const=0;
			//! Get the current direction to the sun.
			virtual vec3 GetDirectionToSun() const=0;
			//! Get the direction to the Sun at the given time.
			virtual vec3 GetDirectionToSun(double t) const=0;
			//! Get the current direction to the Moon.
			virtual vec3 GetDirectionToMoon(sky::uid=0) const=0;
			//! Get the direction to the Moon at the given time.
			virtual vec3 GetDirectionToMoon(sky::uid,double t) const=0;
			//! Get the sunlight that reaches the altitude \a altitude_km.
			virtual vec3 GetLocalSunIrradiance(double t,float altitude_km) const=0;
			//! Get the sunlight that reaches the altitude \a altitude_km.
			virtual float4 GetLocalIrradiance(float altitude_km) const=0;
			//! Get the ratio of Mie to Rayleigh scattering at unit air and haze density. A useful helper function as some shaders use this ratio.
			virtual float4 GetMieRayleighRatio() const=0;
			//! Get the transform vector to convert an altitude in km into a 1D texture coordinate for the sunlight table. x = multiplier, y=offset, so texcoord u=t.x*zkm+t.y;
			virtual vec3 GetSunlightTableTransformKm() const=0;
			//! Get the horizon elevation from a given altitude.
			virtual float GetHorizonElevation(float h_km) const=0;
			//! The Earth's shadow, as seen from the given altitude.
			virtual EarthShadow GetEarthShadow(float h_km,const vec3 &lightDir) const=0;
			//! Get the multiplier for an inscatter factor that gives the total inscatter, where \a cosine is the cosine between
			//! the angle to the sun and the ray direction, \a mie_factor is the proportional haze factor (stored int the w element of the inscatter factor)
			//! and \a alt_km is the viewing altitude in km.
			virtual float4 GetInscatterAngularMultiplier(float cosine,float mie_factor,float alt_km) const=0;
			//! From the altitude specified, the distance in km to 10% visibility (mainly a factor of fog/haze).
			virtual float GetVisibilityDistance(float view_alt_km)=0;
			//! Get the haze that will produce the required visibility distance in the horizontal direction.
			virtual float GetHazeForVisibilityDistance(float view_alt_km,float horiz_dist_km)=0;
			virtual const LightingState &GetLightingState() const=0;
			virtual float GetTemperatureKelvins(float h_km)			const=0;	//!< Return the temperature at the stated altitude in kelvins (subtract 273.15 to get celsius).
			//! When this checksum changes, the atmosphere has been modified sufficiently to warrant regeneration of tables.
			virtual unsigned GetSubdivisionChecksum() const=0;
		};
		//! An abstract interface class for skies.
		SIMUL_SKY_EXPORT_CLASS AtmosphericScatteringInterface
		{
		public:
			//! Calculate Rayleigh coefficients for air using colour wavelengths ColourWavelengthsNm.
			VIRTUAL_GetAndSet(float		,PlanetRadius			,"Radius of the planet, default 6378 km (Earth)")
			VIRTUAL_GetAndSet(float		,AtmosphereThickness	,"Maximum height in Km, effectively where the atmosphere stops.")

			VIRTUAL_GetAndSet(float		,SunRadiusArcMinutes	,"Angular size of the sun.")
		/*! \name General	*/
		/*\*/
			//! Get the direction to the sun.
			virtual vec3 GetDirectionToSun(double epoch_time,float latitude,float longitude) const=0;
			//! Get the moonlight irradiance in space.
			virtual float GetMoonIrradianceFactor(const Moon &m,const vec3 &dir_to_moon,const vec3 &dir_to_sun) const=0;
			//! Get the moonlight that reaches the altitude \a altitude_km.
			virtual vec3 GetLocalMoonIrradianceFactor(const Moon &m,float altitude_km,const vec3 &dir_to_moon,const vec3 &dir_to_sun,HazeStruct hazeStruct) const=0;
			//! Get the sunlight that reaches the altitude \a altitude_km.
			virtual vec3 GetLocalSunIrradiance(float altitude_km,const vec3 &sun_irradiance,const vec3 &dir_to_sun,HazeStruct hazeStruct) const=0;
			
			virtual unsigned GetAtmosphericPropertiesChecksum() const=0;
			virtual unsigned GetAtmosphericStateChecksum() const=0;

			//! Get the loss factor of an object at position \a pos_km seen from \a view_pos_km.

			//! When an object has initial colour \a c, multiply each member of \a c by the corresponding member of
			//! the loss factor to get the colour as seen by the viewer.
			virtual float4 GetColourLossFactor	(const vec3 &view_pos_km,const vec3 &pos_km,HazeStruct hazeStruct,bool earth_test=false,int steps=0) const=0;

			//! The Earth's shadow, as seen from the given altitude.
			virtual EarthShadow GetEarthShadow(float h_km,const vec3 &lightDir) const=0;

			//! Get the loss factor of an object at position \a pos_km seen from \a view_pos_km.
			//! When an object has initial colour \a c, multiply each member of \a c by the corresponding member of
			//! the loss factor to get the colour as seen by the viewer.
			virtual float4 GetIsotropicColourLossFactor	(float alt_km,float elevation,float min_dist_km,float max_dist_km,HazeStruct hazeStruct,bool earth_test=false,int steps=0,bool blackbody=false,float emissivity=0.f) const=0;
			//! Get the inscatter factor in the xyz - the 3-element factor which is multiplied by the 3-element factor Beta to get the inscattered colour.
			//! In the w element is the haze factor.
			//! This excludes skylight and starlight, which are not considered to be strongly anisotropic. See GetIsotropicInscatter().
			virtual float4 GetAnisotropicInscatterFactor	(bool include_sunlight,float alt_km,float elevation,float min_dist_km,float max_dist_km
				,const vec3 &sun_irradiance,vec3 moon_albedo,const float4 &starlight
				,const vec3 &dir_to_sun,const std::map<uid,vec3> &moonDirections
				,HazeStruct hazeStruct,float4 wavelengthsNm
				,const OvercastStruct &overcast,bool earth_test,int steps) const=0;
			//! Get the approximate isotropic inscatter, i.e. the inscatter without a dependence on the angle to the strongest light source. In particular,
			//! this includes skylight and starlight.
			virtual float4 GetIsotropicInscatter(float alt_km,float elevation,float min_dist_km,float max_dist_km
				,const vec3 &sun_irradiance,vec3 moon_albedo,const float4 &starlight
				,const vec3 &dir_to_sun,const std::map<uid,vec3> &moonDirections
				,HazeStruct hazeStruct,float4 wavelengthsNm
				,const OvercastStruct &overcast
				,bool earth_test=false,int steps=0) const=0;
			//! Get the radiance due to blackbody radiation.
			virtual float4 GetBlackbodyInscatter(float alt_km,float elevation,float min_dist_km,float max_dist_km
				,const vec3 &sun_irradiance,vec3 moon_albedo,const float4 &starlight
				,const vec3 &dir_to_sun,const std::map<uid,vec3> &moonDirections
				,HazeStruct hazeStruct,float4 wavelengthsNm
				,bool earth_test=false,int steps=0,float emissivity=0.f) const=0;
			
			//! Get the multiplier for an inscatter factor that gives the total inscatter, where \a cosine is the cosine between
			//! the angle to the sun and the ray direction, \a mie_factor is the proportional haze factor (stored int the w element of the inscatter factor)
			//! and \a alt_km is the viewing altitude in km.
			virtual float4 GetInscatterAngularMultiplier(float cosine,float mie_factor,HazeStruct hazeStruct) const=0;
			//! From the altitude specified, the distance in km to 10% visibility (mainly a factor of fog/haze).
			virtual float GetVisibilityDistance(float view_alt_km,HazeStruct hazeStruct)=0;
			//! Get the haze that will produce the required visibility distance in the horizontal direction.
			virtual float GetHazeForVisibilityDistance(float view_alt_km,float horiz_dist_km,HazeStruct hazeStruct)=0;

			//! Get the amount of ozone present at altitude \a alt_km. This is a dimensionless number.
			virtual float GetOzoneAtAltitude	(float alt_km) const=0;
			//! Get the amount of haze present at altitude \a alt_km compared to the amount at sea level. This value is 
			//! independent of the overall Haze multiplier ( see GetHaze() ).
			virtual float GetHazeFactorAtAltitude(float alt_km,HazeStruct hazeStruct) const=0;
			//! Get the strength of the overcast effect at a given altitude, determined by Overcast, OvercastBaseKm, and OvercastRangeKm.
			virtual float GetOvercastAtAltitude(const OvercastStruct &overcast,float alt_km) const=0;

			virtual float4 GetSunlightFactor	(float alt_km,const vec3 &DirectionToLight,HazeStruct hazeStruct)	const=0;		//!< Return the colour of sunlight passing through the stated altitude as a multiplier of the sun radiance in space.
			
			//! Considering the atmosphere as a shell of thickness GetAtmosphereThickness(), from altitude \a h_km,
			//! this function returns how far a ray cast at angle \a Elevation above the horizon would travel
			//! to reach the outer radius of the shell.
			virtual float GetDistanceToSpace(float Elevation,float h_km) const=0;
			//! From the specified altitude h_km, and in the direction of Elevation, how far is it in Km to the surface?
			virtual float GetDistanceToEarth(float Elevation,float h_km) const=0;
			//! Get the ratio of Mie to Rayleigh scattering at unit air and haze density. A useful helper function as some shaders use this ratio.
		//	virtual float4 GetMieRayleighRatio	() const=0;

		};
	}
}

#ifdef _MSC_VER
    #pragma warning(pop)
#endif
#endif
