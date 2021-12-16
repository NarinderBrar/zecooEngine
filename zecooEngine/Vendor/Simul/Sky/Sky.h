#ifndef SIMUL_SKY_H
#define SIMUL_SKY_H


#include "Simul/Sky/Export.h"
#include "Simul/Sky/Binding.h"
#include "Platform/Core/PropertyMacros.h"
#include "Simul/Sky/Float4.h"
#include "Simul/Sky/SkyInterface.h"
#include "Simul/Sky/SiderealSky.h"
#include "Simul/Sky/Texture1D4.h"
#include "Simul/Sky/Texture2D4.h"
#include "Simul/Sky/Texture2D1.h"
#include "Platform/Math/RandomNumberGenerator.h"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4324)
#endif


namespace simul
{
	//! The namespace and library for Simul sky functionality.
	namespace sky
	{
//! A class that calculates the sky colour.

/*! Sky tracks the time of day, time of year, and viewing location in order to
calculate the relative position of the sun in the sky and hence the lighting.
If you set the year, it can even calculate the position of the moon.
TimeStep() advances the time of day, or you can use SetTime(), or SetHourOfTheDay().

There are three components to an atmosphere in a Simul sky. Most of the colour is determined by the
air density, which is a fixed function of altitude. The air scatters light with Rayleigh scattering, which has three
colour components. You can adjust this scattering using SetDefaultRayleigh(). This value is multiplied
by the density scale to get the final Rayleigh scattering coefficients ( use GetRayleigh() ).

There may also be haze, usually concentrated toward the ground. Haze scatters light differently to air,
it performs Mie scattering, which has a different angular dependence to Rayleigh scattering.

The sky may be overcast, in which case less light reaches the lower altitudes. To achieve this effect, use
SetOvercast(), SetOvercastBaseKm(), and SetOvercastRangeKm() with values obtained from the cloud node.

Finally, Ozone may be present at higher altitudes. Use SetOzoneLevel() to define the amount of ozone (default 0.001),
and SetOzoneLayerAltitude() to determine the altitude where it peaks, in km (default 22km).

The properties of PlanetRadius, AtmosphereThickness and LookupTableSize are generally set only once, or left at their defaults, as
any change will cause a recalculation of the lookup tables. The same goes for Density, which is the ratio of atmospheric density
at sea level to the standard Earth atmosphere.

Other permanent properties can be changed without recalculating the lookups, although it is best to do so infrequently. These include

The MieEccentricity is a value that affects how strongly Mie-scattered light through haze is dispersed. If eccentricity is high (close to 1),
the scattering is preferentially forward, leading to a glow around the sun, whereas if it is low, the haze is distributed more evenly.
	\image html "eccentricity_low.png"
	\image html "eccentricity_high.png"

Sky calculation is performed with ray-tracing, and the number of ray-trace steps is set using \link simul::Sky::SetSteps SetSteps \endlink.
Typically 6 steps are enough, but more may be necessary for high-accuracy, especially when the sun is close to the horizon, such as in this case,
where the number of steps is too low, leading to an over-bright effect on the ground below the sun:
	\image html "sky_steps_too_low.png"

 \section transient_atmospheric_properties Transient Sky Properties

The main factor that changes over time in the sky is the Daytime property, between zero and one. This change can be handled by the
Sky Keyframer, which effectively performs the function of a keyframer for the sky. The Sky Keyframer will also adjust the sky's
Overcast property, a measure of how much the clouds block sunlight from reaching the lower atmosphere. The associated properties
 of OvercastBaseKm and OvercastRangeKm set a base altitude and range in kilometres, at the base altitude, the full overcast
 value is applied, while above the top of the overcast range, there is no loss of light.
 
 The haze value determines how much Mie-scattered haze (i.e. mist or fog) is present. Haze is considered to have a density that falls-off
 exponentially with altitude, so the HazeScaleHeightKm property determines the scaling height for this exponential. The
 HazeBaseHeightKm is also used, below this height, full haze is applied.

 Fog and mist are both effectively low-level clouds. Fog is defined as having visibility less than 1km, it is called mist when visibility
 is between 1 and 2 km.
 
 As time passes and the sun moves, you can use GetAzimuthToSunRadians and GetElevationToSunRadians to find its position.
 The functions GetDirectionToSun and GetDirectionToMoon can also be used.
*/
		SIMUL_SKY_EXPORT_CLASS Sky : public OpticalLengthInterface,public AtmosphericScatteringInterface
		{
		public:
			SiderealSky siderealSky;
			Sky(simul::base::MemoryInterface *mem=NULL);
			virtual ~Sky();
			unsigned GetAtmosphericPropertiesChecksum() const;
			unsigned GetAtmosphericStateChecksum() const;
			META_BeginProperties
				META_RangePropertyWithSetCall(int		,LookupTableSize		,16,256			,RemakeSkyLookupTable	,"Size, from 16 to 256, of the table used to store densities and optical lengths.")
				META_RangePropertyWithSetCall(float		,AtmosphereThickness	,0.f	,100.f		,AtmosphereThicknessChanged	,"Maximum height in Km.")
				META_RangePropertyWithSetCall(float		,PlanetRadius			,100.f	,200000.f	,PlanetRadiusChanged		,"Radius in km of the planet, default=6378km (Earth).")
				META_RangePropertyWithSetCall(int		,Steps					,1		,100		,StepsChanged				,"The number of integration steps taken to calculate results.")
				META_RangeProperty(float				,SunRadiusArcMinutes	,16.f,0.f,3600.f			,"Angular size of the sun.")
			META_EndProperties

			float GetHorizonElevation(float h_km) const;
			EarthShadow GetEarthShadow(float h_km,const vec3 &lightDir) const;

			static float Blackbody(float T_K,float l_nm);

			/// Moon irradiance as a multiple of sun irradiance.
			float GetMoonIrradianceFactor				(const Moon &m,const vec3 &dir_to_moon,const vec3 &dir_to_sun) const;
			float4	GetColourLossFactor					(const vec3 &view_pos_km,const vec3 &pos_km,HazeStruct hazeStruct,bool earth_test=false,int steps=0) const;
			float4	GetIsotropicInscatter				(float alt_km,float elevation,float min_dist_km,float max_dist_km,const vec3 &sun_irradiance,vec3 moon_albedo,const float4 &starlight,const vec3 &dir_to_sun,const std::map<uid,vec3> &moonDirections,HazeStruct hazeStruct,float4 wavelengthsNm,const OvercastStruct &overcast,bool earth_test=false,int steps=0) const;
			float4	GetBlackbodyInscatter				(float alt_km,float elevation,float min_dist_km,float max_dist_km,const vec3 &sun_irradiance,vec3 moon_albedo,const float4 &starlight,const vec3 &dir_to_sun,const std::map<uid,vec3> &moonDirections,HazeStruct hazeStruct,float4 wavelengthsNm,bool earth_test=false,int steps=0,float emissivity=0.f) const;
			float	GetOzoneAtAltitude					(float alt_km) const;

			float4 GetIsotropicColourLossFactor			(float view_alt_km,float elevation,float min_dist_km,float max_dist_km,HazeStruct hazeStruct,bool earth_test=false,int steps=0,bool do_blackbody=false,float emissivity=0.f) const;
			float4 GetAnisotropicInscatterFactor		(bool include_sunlight,float view_alt_km,float elevation
				,float min_dist_km,float max_dist_km
				,const vec3 &sun_irradiance,vec3 moon_albedo,const float4 &starlight
				,const vec3 &dir_to_sun,const std::map<uid,vec3> &moonDirections
				,HazeStruct hazeStruct
				,float4 wavelengthsNm,const OvercastStruct &overcast,bool earth_test,int steps) const;
			float4 GetInscatterAngularMultiplier		(float cos0,float mie_factor,HazeStruct hazeStruct) const;

			float GetVisibilityDistance(float view_alt_km,HazeStruct hazeStruct);
			float GetHazeForVisibilityDistance(float view_alt_km,float horiz_dist_km,HazeStruct hazeStruct);

			
			vec3 GetDirectionToLight		(double epoch_time,float latitude,float longitude,float altitude_km) const;
			//! Get the direction the sun represented as a vector
			vec3 GetDirectionToSun			(double epoch_time,float latitude,float longitude) const;
			
			float GetTemperatureKelvins(float SeaLevelTemperatureK,float h_km) const;
			float GetDensityFactorAtAltitude(float h_km) const;
			float GetDensityAtAltitude(float h_km) const; // Density in kg/m^3 at altitude \a h_km (kilometres).
			const float4 *GetDensityTable() const;
			const float4 *GetOpticalDepthTable() const;
			const float4 *GetBlackbodyTable(float4	ColourWavelengthsNm) const;
			//! A checksum that changes any time the density, optical depth or blackbody table is recalculated.
			unsigned GetTablesChecksum() const;
			// OpticalLengthInterface
			float GetOpticalLength(float Elevation,float h_km,float distance_to_edge) const;
			//! Get the effective optical length (equivalent length at sea level density) of a ray
			//! cast from altitude \a h_km at angle \a Elevation above the horizon.
			float GetOpticalLength(float Elevation,float h_km) const;
			//! Get the effective optical length of ozone of a ray cast from altitude \a
			//! h_km at angle \a Elevation above the horizon.
			float GetOzoneOpticalLength(float Elevation,float h_km) const;

			static float HenyeyGreenstein(float g,float cos0);

			void InitLookupTables();
			
		protected:
			///
			enum IntegrateMode
			{
				SUNLIGHT,SKYLIGHT,BLACKBODY
			};
			float4 Integrate(IntegrateMode integrateMode,float view_alt_km,float elevation
				,float min_dist_km,float max_dist_km
				,const vec3 &sun_irradiance
				,vec3 moon_albedo
				,const float4 &starlight
				,const vec3 &dir_to_sun
				,const std::map<uid,vec3> &moonDirections
				,HazeStruct hazeStruct,const float4 &wavelengthsNm
				,const OvercastStruct &overcast
				,bool earth_test,int steps,float emissivity=0.f) const;
		
			virtual void AtmosphereThicknessChanged();
			virtual void PlanetRadiusChanged();
			mutable void* sky_mutex;
			mutable void* dens_mutex;
			vec3 UpdateSunPosition(double epoch_time,float latitude,float longitude) const;
			void MakeHazeLookupTable();
			bool MakeSkyLookupTable() const;
			void RemakeSkyLookupTable() const;
			void MakeDensityLookupTable() const;

			mutable Texture2D4 sky_lookup;
			mutable Texture1D4 blackbody_table;
			mutable Texture1D4 density_lookup;
			mutable Texture2D1 haze_optical_depth_lookup;
			mutable bool density_lookup_valid;
			mutable bool sky_lookup_valid;
			mutable bool blackbody_table_valid;
			float GetHazeOpticalLength(float Elevation,float h_km,HazeStruct hazeStruct) const;
			void Lookup(float Elevation,float h_km,float &opt_len,float &mean_alt,float &distance_to_edge) const;
			void LookupDensities(float h_km,float &dens,float &ozone,float &haze_factor,HazeStruct hazeStruct) const;

			virtual void StepsChanged();
			float *step_centres;
			float *step_lengths;
			void ChangedMieCoefficients();
			//BaseSky
			void WavelengthsChanged();
			float4 GetSunlightFactor(float alt_km,const vec3 &directionToLight,HazeStruct hazeStruct) const;
			float4 GetSunlightFactor(const vec3 &Pos,const vec3 &directionToLight,HazeStruct hazeStruct) const;
			simul::base::MemoryInterface *memoryInterface;
			mutable unsigned tables_checksum;
		public:
			static vec3 AzimuthElevationToDirection(float az,float el);
			float	GetHazeFactorAtAltitude				(float alt_km,HazeStruct hazeStruct) const;
			float	GetOvercastAtAltitude				(const OvercastStruct &o,float alt_km) const;
			float4 GetAmbientLight						(float4 wavelengthsNm,float altitude_km,const vec3 &sun_irradiance,vec3 moon_albedo,const float4 &starlight,const vec3 &dir_to_sun,const std::map<uid,vec3> &moonDirections,HazeStruct hazeStruct,const OvercastStruct &o) const;
			vec3 GetLocalSunIrradiance					(float altitude_km,const vec3 &sun_irradiance,const vec3 &DirectionToSun,HazeStruct hazeStruct) const;
			vec3 GetLocalMoonIrradianceFactor			(const Moon &m,float altitude_km,const vec3 &moonDirection,const vec3 &dir_to_sun,HazeStruct hazeStruct) const;
			float GetShortestDistanceToAltitude			(float Elevation,float start_h_km,float finish_h_km) const;
			float GetLongestDistanceToAltitude			(float Elevation,float start_h_km,float finish_h_km) const;
			float GetDistanceToSpace					(float Elevation,float h_km) const;
			float GetDistanceToEarth					(float Elevation,float h_km) const;
		protected:
			float GetDistanceToAltitude			(float Elevation,float start_h_km,float finish_h_km,bool use_longer_distance) const;
			//! Get the effective optical length of haze of a ray cast from altitude \a
			//! h_km at angle \a Elevation above the horizon.
			float GetHazeFactorOpticalLength(float Elevation,float h_km,HazeStruct hazeStruct) const;
			float GetOvercastAtAltitudeRange(const OvercastStruct &o,float alt_km,float prev_alt_km) const;
			float GetEclipseFactor(const vec3 &DirectionToSun,const vec3 &DirectionToMoon,const Moon &m) const;
			//BaseSky
			void HazeBaseHeightChanged() ;
			void HazeScaleHeightChanged() ;
			void SunIrradianceChanged()  {}
		};
		extern SIMUL_SKY_EXPORT void CalcAutomaticRayleighCoefficients(float4 &r,const float4 &ColourWavelengthsNm);
		//! Calculates the mie coefficients (r,g,b) due to aerosol haze.
		extern SIMUL_SKY_EXPORT float4 CalcMieCoefficients(float4 ColourWavelengthsNm,float haze);
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif
