#ifndef SIMUL_ALTITUDEFADETABLE_H
#define SIMUL_ALTITUDEFADETABLE_H

#include "Simul/Sky/Export.h"
#include "Simul/Sky/Binding.h"
#include "Platform/Core/PropertyMacros.h"
#include "Simul/Sky/Float4.h"
#include "Simul/Sky/SkyInterface.h"
#include "Simul/Sky/Texture1D4.h"
#include "Simul/Sky/Texture2D4.h"
#include "Simul/Sky/SkyTexturesCallback.h"

#include "Simul/Sky/BaseKeyframer.h"
#include "Simul/Sky/Stars.h"
#include "Simul/Sky/CpuSkyGenerator.h"

#include "Simul/Base/Referenced.h"
#include "Platform/Core/ProfilingInterface.h"
#include "Simul/Sky/Sky.h"
#include <vector>
#include <stack>
#include <map>
#include <string>

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

namespace simul
{
	namespace sky
	{
		class AtmosphericScatteringInterface;
		class Sky;
		class OvercastCallback;
		class Aurora;

		struct GpuSkyParameters;
		struct GpuSkyInfraredParameters;
		struct GpuSkyAtmosphereParameters;
		struct CelestialOverrides
		{
			float SunAzimuth=0.f;
			float SunElevation = 0.f;
			float MoonAzimuth = 0.f;
			float MoonElevation = 0.f;
			mutable unsigned int checksum = 0;
		};

/*! A class that maintains sky properties and interpolates them based on keyframe values.

	An instance of the SkyKeyframer is kept as a member of the Environment instance.
	The SkyKeyframer interpolates the sky values, and passes the keyframe and interpolation data to external renderers,
	e.g. \link simul::sky::BaseSkyRenderer BaseSkyRenderer\endlink and \link simul::sky::BaseAtmosphericsRenderer BaseAtmosphericsRenderer\endlink.
	The generated tables are calculated for multiple altitudes - specified with SetNumAltitudes.
The SunIrradiance value determines the colour and strength of sunlight, and is expressed in physical units. The apparent size of the sun is
set using SetSunRadiusArcMinutes.
*/
		SIMUL_SKY_EXPORT_CLASS SkyKeyframer : public BaseKeyframer, public BaseSkyInterface
		{
		public:
			SkyKeyframer(simul::base::MemoryInterface *mem,int num_elev=128,int num_dist=64,float max_dist_km=300.f);
			virtual ~SkyKeyframer();
			virtual const SkyKeyframer &operator=(const SkyKeyframer& aft);
			META_BeginProperties
				META_RangePropertyWithSetCall(float		,MaxDistanceKm,5.f,5000.f	,SetRecalculate					,"The maximum distance for the fade.")		
				META_RangePropertyWithSetCall(float		,MinAltitudeKm,-1.0f,100.f	,SetRecalculate					,"The minimum altitude that will be used for fade and sky colour calculations. Few aircraft can fly higher than 15km.")
				META_RangePropertyWithSetCall(float		,MaxAltitudeKm,-1.0f,100.f	,SetRecalculate					,"The maximum altitude that will be used for fade and sky colour calculations. Few aircraft can fly higher than 15km.")
				META_RangePropertyWithSetCall(int		,NumAltitudes,1,64			,SetRecalculate					,"The number of altitudes to calculate.")

				META_PropertyWithSetCall(bool			,EarthTest					,SetRecalculate					,"Whether light should stop at the planet's surface - normally false.")
				META_RangePropertyWithSetCall(int		,NumElevations,2,2048		,SetRecalculate					,"The height of the table, i.e. the number of elevation steps.")
				META_RangePropertyWithSetCall(int		,NumDistances,2,512			,SetRecalculate					,"The width of the table, i.e. the number of distance steps.")
				META_Property(bool						,RecalculateAtNextFrame										,"If true, changes to global properties like latitude and longitude are incorporated at the next calculated keyframe. If false, they are recalculated instantly.")
				META_Property(int						,StartDayNumber												,"Day Number from 12am, January 1st, 2000")
				META_PropertyWithSetCall(bool			,LinkKeyframeTimeAndDaytime	,LinkChanged					,"Whether to make daytime the same as a keyframe's time value. Otherwise, daytime is an independent value, relative to the start date.")
				META_Property(bool						,ShowEarthShadow											,"Whether shaders should show the shadow of the Earth on the sky.")
				META_RangePropertyWithSetCall(float		,BrightnessPower			,0.00001f,1.f,SetRecalculate	,"A power to raise light sources to, to adjust for extreme brightness and darkness. 1 is linear, smaller values reduce the ratio between bright and dark scenes.")
				
				META_Property(vec3						,SunIrradiance												,"RGB irradiance of the sun in space, in watts per square metre per micrometre.")
				META_PropertyWithSetCall(float			,MaxStarMagnitude			,StarsChanged					,"The maximum magnitude for stars - larger values are dimmer, the human eye can see up to magnitude 6 or 7.")
				META_RangeProperty(float				,Emissivity					,0.f	,0		,2.f			,"Blackbody emissivity.")
				META_Property(std::string				,SunTexture													,"A texture to be drawn for the sun - does not usually include path information.")
				META_Property(std::string				,BackgroundTexture											,"A texture to be drawn as a background; can be a cubemap or a latitude-longitude spherical projection")
				META_Property(float4					,Starlight													,"Brightness and colour of the stars.")
				META_PropertyWithSetCall(bool			,UrsaMajor					,StarsChanged					,"If true, only the stars of Ursa Major will be shown - useful for calibrating ephemerides.")
				META_RangeProperty(double				,LatitudeRadians	,0,-3.1415926536/2.0,3.1415926536/2.0	,"Latitude in radians.")
				META_RangeProperty(double				,LongitudeRadians	,0,-3.1415926536	,3.1415926536		,"Longitude in radians.")
				META_Property(float						,TimezoneHours												,"Timezone in hours (+-) relative to UST. SetTime() and GetTime() are considered to be local times.")
				META_PropertyOverride(float4			,ColourWavelengthsNm										,"Red, green and blue wavelengths in nanometres.")
				META_Property(float4					,Rayleigh													,"Rayleigh scattering coefficients at standard sea level density, xyz = red, green, blue.")
				META_Property(vec4						,BaseOzone													,"Ozone absorption coefficients, xyz = red, green, blue.")
				META_RangeProperty(float				,OzoneStrength			,0.05f,0.f	,.5f					,"Amount of Ozone in the atmosphere.")
				META_Property(float						,MaxSunRadiance												,"Maximum practical brightness for sun rendering.")
				META_Property(bool						,AdjustSunRadius											,"Whether to adjust the apparent radius of the sun if its radiance exceeds MaxSunRadiance.")
				META_Property(float						,HeadingOffsetRadians										,"Direction of the y axis clockwise from North.")
			
				META_Property(bool						,SaveCelestialProperties									,"Whether to save moons etc.")
				META_Property(bool						,SaveLatLong												,"Whether to save Latitude and Longitude.")
			META_EndProperties
				
			enum class Enum
			{
				DAWN = 50001
				, SUNSET = 50002
			};

			static long long GetEnum(const char *n);
			base::Variant GetValue(long long Enum) const;

			//! What date is referred to as time zero.
			void GetStartDate(int &y,int &m,int &d) const;
			//! Set the date for time=0.
			void SetStartDate(int y,int m,int d);

			float GetPlanetRadiusKm() const
			{
				return skyNode.GetPlanetRadius();
			}
			//! The elevation of the horizon at this altitude
			float GetHorizonElevation(float h_km) const override;

			//! Calculate SunIrradiance using the solar irradiance tables outside Earth's atmosphere, based on ColourWavelengthsNm.
			void CalcSunIrradianceAtEarth();

			void SetSkyInterface(AtmosphericScatteringInterface *s);
			AtmosphericScatteringInterface *GetSkyInterface();
			const AtmosphericScatteringInterface *GetSkyInterface() const;
			SiderealSkyInterface *GetSiderealSkyInterface();
			const SiderealSkyInterface *GetSiderealSkyInterface() const;
			void GetSiderealTransform(float *m,float latitude_radians,float longitude_radians) const;

			virtual float GetDaytime() const;

			float GetDawn(float latitude_radians,float longitude_radians) const;
			float GetSunset(float latitude_radians,float longitude_radians) const;

			//! Set the current direction of the sun by it's Azimuth and Elevation
			void OverrideSunDirection(float az, float el);
			//! Set the current direction of the moon by it's Azimuth and Elevation
			void OverrideMoonDirection(float az, float el);
			
			/*!Return true if the keyframer has the floating point value with the given, case-insensitive, name; return false otherwise.
			
			These are the properties the SkyKeyframer has:
					- LatitudeRadians
					- LongitudeRadians
					- LatitudeDegrees
					- LongitudeDegrees
					- MaxStarMagnitude
					- MinimumStarPixelSize
					- MaxDistanceKm
					- MaxAltitudeKm
					- atmosphereThicknessKm
					- planetRadiusKm
					- MaxSunRadiance
					- BrightnessPower
					- OzoneStrength
					- Emissivity
					- MoonAlbedo
					- TimezoneHours
					- SunRadiusArcMinutes
					- MoonRadiusArcMinutes
					- sunazimuth
					- sunelevation
					- moonazimuth
					- moonelevation
					- sunazimuthdegrees
					- sunelevationdegrees
					- moonazimuthdegrees
					- moonelevationdegrees
					- sunirradiance
					- colourwavelengthsnm
			*/
			bool HasFloat(const char *name) const override;
			//! Get a float with the given, case-insensitive, name
			float GetFloat(const char *name,const base::Variant *params=NULL) const override;
			float GetDefaultFloat(const char *name) const override;
			//! Set a float with the given, case-insensitive, name
			void SetFloat(const char *name,float val) override;

			/*!Return true if the keyframer has an integer or true/false value with the given, case-insensitive, name; return false otherwise. 
			
			These are the properties the SkyKeyframer has:
				- StoreAsColours
				- AutoMie
				- numColourAltitudes
				- numColourElevations
				- numColourDistances
				- AutomaticSunPosition
			*/
			bool HasInt(const char *name) const override;
			//! Get an int with the given, case-insensitive, name
			int GetInt(const char *name,const base::Variant *params=NULL) const override;
			int GetDefaultInt(const char *name) const override;
			//! Set an int with the given, case-insensitive, name
			void SetInt(const char *name,int val) override;
			
			//! Set a value with the given enum
			void Set(const char *name, const base::Variant *params) override;

			virtual void SortKeyframes() override;

			double DaytimeToEpochTime(float t) const;

			float EpochTimeToDaytime			(double e) const;
			float GetCurrentStarBrightness		() const;
			float GetCurrentBackgroundBrightness() const;
			// Implementing BaseSkyInterface:
			virtual const float &GetTime				() const override;
			virtual void SetTime				(const float &d) override;
			virtual const float &GetMieEccentricity	() const override;
			virtual vec3 GetDirectionToLight	(float alt_km) const override;
			virtual vec3 GetDirectionToLight	(double time,float alt_km) const override;
			virtual vec3 GetDirectionToSun	() const override;
			virtual vec3 GetDirectionToSun	(double time) const override;
			virtual vec3 GetDirectionToMoon	(uid u=0) const override;
			virtual vec3 GetDirectionToMoon	(uid u,double time) const override;
			/// Direction in Earth-centred equatorial co-ordinates, where Z=Earth's North axis, X is 90 deg East and Y is 180 degrees.
			vec3 GetGeocentricDirectionToMoon	(uid u) const ;
			vec3 GetGeocentricDirectionToMoon	(uid u,double time) const ;
			vec3 GetGeocentricDirectionToSun	() const;

			vec3 GetGeocentricDirectionToSun	(double time) const;
			double GetSiderealTimeDegrees() const;
			double GetSiderealTimeDegrees(double time) const;
			//! Get the direction the sun represented as a vector
			vec3 GetDirectionToSun(int keyframe) const;

			vec3 GetMoonRadiance(uid u) const;

			virtual vec4	GetMoonIrradiance			(uid u) const;
			virtual float4	GetLocalIrradiance			(float altitude_km) const override;
			virtual vec3	GetLocalSunIrradiance		(double t,float altitude_km) const override;
			virtual vec3	GetLocalMoonIrradiance		(uid u, double t, float altitude_km) const;
			virtual float4	GetLocalIrradiance			(double time,float altitude_km) const;
			virtual float4	GetMieRayleighRatio			() const override;
			virtual vec3	GetSunlightTableTransformKm	() const override;

			virtual float4	GetIsotropicColourLossFactor	(float alt_km,float elevation,float min_dist_km,float max_dist_km,bool earth_test=false,int steps=0) const;
			virtual float4	GetAnisotropicInscatterFactor	(bool include_sunlight,float alt_km,float elevation,float min_dist_km,float max_dist_km,bool earth_test,int steps) const;
			virtual float4	GetInscatterAngularMultiplier	(float cosine,float mie_factor,float alt_km=0) const override;
			virtual float	GetVisibilityDistance			(float view_alt_km) override;
			virtual float	GetHazeForVisibilityDistance	(float view_alt_km,float horiz_dist_km) override;
			float GetFogDensityGm3ForVisibilityDistanceKm(float horiz_dist_km);
			void			SetAltitudeRangeKm				(float min_km,float max_km);
			void			TimeStep						(float dt);
			virtual float	GetDistanceToSpace				(float Elevation,float h_km) const;
			virtual float	GetDistanceToEarth				(float Elevation,float h_km) const;
			float GetLastTimeStep() const;

			//! Per-frame update call, this function updates the interpolation and the tables.
			virtual void Update();

			void ApplyAutoKeyframeValues();

			virtual void Reset();
			// The Earth's shadow
			EarthShadow GetEarthShadow(float h_km,const vec3 &lightDir) const override;
			//! Returns the texture coordinate for the given altitude - constant per-frame.
			float GetAltitudeTexCoord(float h_km) const;

			void SetOvercastCallback(simul::sky::OvercastCallback *c);
			simul::sky::OvercastCallback *GetOvercastCallback();
			void SetRecalculate();
			void KeyframesChanged() override
			{
				SetRecalculate();
			}
			//! Create a number of evenly spaced keyframes.
			void SetUniformKeyframes			(int Steps,float range=1.f);
			float GetHazeForVisibilityDistance(SkyKeyframe *K,float alt_km,float dist_km);
			float GetVisibilityDistance			(SkyKeyframe *K,float alt_km);

			float GetDaytimeForKeyframe	(int i) const;
			float GetDaytimeForTime		(float t) const;

			const std::map<uid,Moon> &GetMoons() const
			{
				return moons;
			}
			Moon* GetMoon(id i);
			void ClearMoons();
			id SetMoon(id i,const Moon &m) ;
			void RemoveMoon(id i);

			//! Get the per-frame cached lighting state 
			const LightingState &GetLightingState() const override
			{
				return lightingState;
			}
			const KeyframeInUseState &GetKeyframeInUseState(int s) const
			{
				const KeyframeInUseState &U	=keyframeInUseStates[(texture_cycle+s)%3];
				return U;
			}
			const LightingState &GetSubdivisionLightingState(int s) const
			{
				const KeyframeInUseState &U	=keyframeInUseStates[(texture_cycle+s)%3];
				return U.lightingState;
			}
			float GetTemperatureKelvins(float h_km)			const override;	
			const OvercastStruct &GetCurrentOvercastState() const
			{
				return overcastStruct;
			}
			BaseKeyframe *InsertKeyframe(float t) override;
			//! Delete a skykeyframe with the given ID
			void DeleteKeyframe(int i);
			//! The next keyframe not partially built or being used. This keyframe's properties can be modified without any pause or rebuild.
			SkyKeyframe *GetNextModifiableKeyframe();
			const sky::BaseKeyframe *GetNextModifiableKeyframe(float time) const override;
			const BaseKeyframe *GetInterpolatedBaseKeyframe() const override
			{
				return &interpolatedKeyframe;
			}
			//! Get the current \em interpolated keyframe, which holds the values interpolated from the two surrounding keyframes at
			//! any given time.
			SkyKeyframe *GetInterpolatedKeyframe()
			{
				return &interpolatedKeyframe;
			}
			const SkyKeyframe *GetInterpolatedKeyframe() const
			{
				return &interpolatedKeyframe;
			}
			virtual void Save(Output &os) const override;
			virtual void Load(Input &is) override;
			virtual void SaveToText(crossplatform::TextOutput &output,bool include_keyframes) const override;
			virtual void LoadFromText(crossplatform::TextInput &input) override;
			// Clear the keyframe sequence:
			virtual void New();

			//! Convert a floating-point daytime into hour, minute, second and milliseconds.
			static void DaytimeToClockTime(float &t,int &d,int &h,int &m,int &s,int &ms);
			Stars stars;
			
			//! Get the GPU sky parameters corresponding to the given subdivision triplet index, and factor in eclipses and brightness power modifiers.
			void GetGpuSkyParameters(GpuSkyParameters &p,GpuSkyAtmosphereParameters &a,GpuSkyInfraredParameters &ir,int index);

			//! Get the GPU sky parameters corresponding to the given keyframe at the stated time.
			void GetGpuSkyParameters(GpuSkyParameters &p, GpuSkyAtmosphereParameters &a, GpuSkyInfraredParameters &ir, const SkyKeyframe *K, float daytime, float complete);
			

			int3 GetTextureSizes();

			void GenerateColours(SkyKeyframe *K);

			const BaseKeyframe *GetInterpolationStartKeyframe() const
			{
				if(SubdivisionMethod==FIXED_REALTIME)
					return &interpolationStartKeyframe;
				else
				{
					KeyframeInterpolation interpolationStruct=GetKeyframeInterpolation(interpolatedKeyframe.time);
					return static_cast<const SkyKeyframe *>(GetKeyframe(interpolationStruct.keyframe));
				}
			}
			BaseKeyframe *GetInterpolationStartKeyframe()
			{
				if(SubdivisionMethod==FIXED_REALTIME)
					return &interpolationStartKeyframe;
				else
				{
					KeyframeInterpolation interpolationStruct=GetKeyframeInterpolation(interpolatedKeyframe.time);
					return static_cast<SkyKeyframe *>(GetKeyframe(interpolationStruct.keyframe));
				}
			}
			const BaseKeyframe *GetInterpolationEndKeyframe() const
			{
				if(SubdivisionMethod==FIXED_REALTIME)
					return &interpolationEndKeyframe;
				else
				{
					KeyframeInterpolation interpolationStruct=GetKeyframeInterpolation(interpolatedKeyframe.time);
					return static_cast<const SkyKeyframe *>(GetKeyframe(interpolationStruct.keyframe+1));
				}
			}
			BaseKeyframe *GetInterpolationEndKeyframe()
			{
				if(SubdivisionMethod==FIXED_REALTIME)
					return &interpolationEndKeyframe;
				else
				{
					KeyframeInterpolation interpolationStruct=GetKeyframeInterpolation(interpolatedKeyframe.time);
					return static_cast<SkyKeyframe *>(GetKeyframe(interpolationStruct.keyframe+1));
				}
			}

			virtual const BaseKeyframe *GetSubdivisionKeyframe(int index) const override;
			virtual BaseKeyframe *GetSubdivisionKeyframe(int index) override;

			BaseKeyframe *GetKeyframe(int index) override;
			const BaseKeyframe *GetKeyframe(int i) const override;

			/*/// Highlight the named constellation in debug views:
			void HighlightConstellation(const char *);
			/// Remove the named constellation from the highlighted list
			void UnHighlightConstellation(const char *);
			/// Clear the highlighted constellations
			void ClearHighlightConstellations();
			/// Get the set of highlighted constellations
			const std::set<std::string> GetHighlightConstellations() const;*/
			/// This is a checksum that only changes if an in-use subdivision has modified (cycling does not change it).
			unsigned GetSubdivisionChecksum() const override;
			virtual bool IsKeyframeInUse(int k) const override;
			Aurora* aurora = nullptr;
			Sky& GetSkyNode() { return skyNode; }
			const SkyKeyframeOverrides			*GetSkyKeyframeOverrides()
			{
				return &skyKeyframeOverrides;
			}
			void SetSkyKeyframeOverrides(SkyKeyframeOverrides shouldOverride)
			{
				skyKeyframeOverrides = shouldOverride; //not currently overriding Moons, but leaving functionality
			}

		protected:
			void SubdivisionMethodChanged() override;
			void StarsChanged();
			float CalcMaxDistanceKm(float alt_km,float elev) const;
			virtual void LinkChanged();
			// For arbitrary altitude:
			float4 GetIsotropicInscatter(float alt_km,float elevation,float min_dist_km,float max_dist_km,bool earth_test=false,int steps=0) const;
			float4 GetBlackbodyInscatter(float alt_km,float elevation,float min_dist_km,float max_dist_km,bool earth_test=false,int steps=0,float emissivity=0.f) const;
			float4 GetIsotropicInscatter(float view_alt_km,float elevation,float dist_km,bool earth_test,int steps) const;
			// For current view altitude:
			float4 GetIsotropicColourLossFactor	(float elevation,float dist_km) const;
			float4 GetAnisotropicInscatterFactor	(float elevation,float dist_km) const;
			block_texture_fill FillFadeTex(int texel_index,int num_texels
				,const float *loss_float4_array,const float *inscatter_float4_array,const float *skylight_float4_array);
			//void SetOvercastChecksum(SkyKeyframe *K);
			AtmosphericScatteringInterface	*atmosphericScatteringInterface;
			SiderealSkyInterface			*siderealSkyInterface;
			simul::sky::OvercastCallback	*overcastCallback;
			SkyKeyframe						interpolatedKeyframe;
			/// Apply any overrides.
			bool							InterpolateKeyframe(SkyKeyframe *K) const;
			bool							InterpolateKeyframe(SkyKeyframe* K,const SkyKeyframe* K1,const SkyKeyframe* K2,float interp) const;
			bool							InterpolateKeyframeFromSubdivisions(SkyKeyframe* K) const;
			void							CycleTexturesForward() override;
			void							CycleTexturesBackward() override;
			void							EnsureKeyframeCalculationForSubdivision(int index);
			void							EnsureSubdivisionValidity(int i) override;
			void							EnsureUseStateValidity(KeyframeInUseState &U,const SubdivInterpolation &s,int index) const;
			void							GetUseStates(const SubdivInterpolation &s,const KeyframeInUseState *&U1,const KeyframeInUseState *&U2) const;
			virtual void					StoreSky();
			virtual void					ApplySkyKeyframe(int i);
			virtual void					RestoreSky();
			virtual void					ChangedSkyInterface();
			void							InitFadeTextures();	
			virtual unsigned				GetGlobalChecksum() const override;
			
			SkyKeyframe						defaultKeyframe;
			CelestialOverrides				celestialOverrides;
			SkyKeyframe						interpolationStartKeyframe;
			SkyKeyframe						interpolationEndKeyframe;
			SkyKeyframeOverrides			skyKeyframeOverrides;
			bool							initialized;
			int								day;
			std::stack<float>				old_t;
			mutable bool					recalc;

			unsigned						last_checksum;
			unsigned						last_global_checksum;
			unsigned						force_recalc;
			float							CalcBrightnessForKeyframe(const SkyKeyframe *K) const;
			KeyframeInUseState				keyframeInUseStates[3];
			mutable KeyframeInUseState		tempStates[2];
			void							CalcLightingState();

			LightingState					lightingState;
			SubdivInterpolation				currentSubdivStruct;
			OvercastStruct					overcastStruct;
			Sky								skyNode;
			//std::set<std::string>			highlightConstellations;
			float							last_time_step;
			std::map<uid,Moon>				moons;
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif