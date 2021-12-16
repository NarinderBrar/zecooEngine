#ifndef SIMUL_GRAPH_CLOUDKEYFRAMER_H
#define SIMUL_GRAPH_CLOUDKEYFRAMER_H
#include "Platform/Core/PropertyMacros.h"
#include "Simul/Base/Referenced.h"
#include "Simul/Base/ShowProgressInterface.h"
#include "Platform/Core/MemoryInterface.h"
#include "Simul/Clouds/Export.h"
#include "Simul/Clouds/CloudInterface.h"
#include "Simul/Clouds/CellularCloudNode.h"
#include "Simul/Sky/OvercastCallback.h"
#include "Simul/Sky/BaseKeyframer.h"
#include "Simul/Clouds/GpuCloudGenerator.h"
#include "Platform/Math/RandomNumberGenerator.h"
#include "Simul/Sky/SkyKeyframer.h"
#include "Simul/Clouds/Storm.h"
#include <vector>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

#define SECONDS_PER_DAYF (24.f*60.f*60.f)

namespace simul
{
	namespace base
	{
		class ProfilingInterface;
	}
	namespace sky
	{
		class BaseSkyInterface;
	}
	namespace base
	{
		class ShowProgressInterface;
	}
	namespace clouds
	{
		/// A struct to identify selections of keyframes, layers etc.
		struct SIMUL_CLOUDS_EXPORT SelectionIdentifier
		{
			///
			enum Type
			{
				UNKNOWN = 0,
				SKY = 1,
				CLOUD_LAYER = 2,
				SKY_KEYFRAME = 4,
				CLOUD_KEYFRAME = 8,
				STORM = 16,
				OCEAN = 32,
				VOLUME = 64,
				ALL=0xFFFFFFFF
			};
			SelectionIdentifier(Type t);
			SelectionIdentifier(const SelectionIdentifier& kfi);
			const SelectionIdentifier& operator=(const SelectionIdentifier& kfi);
			friend bool SIMUL_CLOUDS_EXPORT operator<(const SelectionIdentifier& kfi1, const SelectionIdentifier& kfi2);
			bool operator!=(const SelectionIdentifier& kfi);
			bool operator==(const SelectionIdentifier& kfi);
			Type type;
			sky::uid layer_uid;
			sky::uid uid;
			float pos;
		};
		class CloudInterface;
		//! A class to manage interpolation between cloud states over time.
		// The type of rendering we are doing in a given call.
		enum class TrueSkyRenderMode
		{
			DEFAULT=0
			,CUBEMAP=1
			,VR=2
			,VR_ALTERNATE_EYE=4
			,DRAW_SUN=8
			,DRAW_PLANETS=16
			,DRAW_STARS=32
			,DRAW_COSMOS=64
			,DRAW_LIGHTNING=128
			,DRAW_PRECIPITATION=256
			,DRAW_ATMOSPHERE=512
			,VR_BOTH_EYES=1024
			,DEPTH_BLENDING=2048
			,DRAW_CLOUDS=4096
			,WRITE_BLEND_ALPHA=8192
			,DRAW_AURORA=16384
			,STANDARD=DRAW_SUN|DRAW_PLANETS|DRAW_STARS|DRAW_COSMOS|DRAW_ATMOSPHERE|DRAW_CLOUDS|DEPTH_BLENDING
			,STANDARD_CUBEMAP=CUBEMAP|DRAW_ATMOSPHERE|DRAW_CLOUDS
			,SKYBOX_CUBEMAP=CUBEMAP|DRAW_ATMOSPHERE|DRAW_CLOUDS|DRAW_SUN|DRAW_PLANETS|DRAW_STARS|DRAW_COSMOS
			,TRANSLUCENT= DRAW_LIGHTNING| DRAW_PRECIPITATION
		};
		inline TrueSkyRenderMode operator&=(TrueSkyRenderMode &a, const TrueSkyRenderMode &b)
		{
			a = static_cast<TrueSkyRenderMode>(static_cast<int>(a) & static_cast<int>(b));
			return a;
		}
		inline TrueSkyRenderMode operator|=(TrueSkyRenderMode &a, const TrueSkyRenderMode &b)
		{
			a=static_cast<TrueSkyRenderMode>(static_cast<int>(a) | static_cast<int>(b));
			return a;
		}
		inline TrueSkyRenderMode operator|(TrueSkyRenderMode a, TrueSkyRenderMode b)
		{
			return static_cast<TrueSkyRenderMode>(static_cast<int>(a) | static_cast<int>(b));
		}
		inline TrueSkyRenderMode operator&(TrueSkyRenderMode a, TrueSkyRenderMode b)
		{
			return static_cast<TrueSkyRenderMode>(static_cast<int>(a) & static_cast<int>(b));
		}
		inline TrueSkyRenderMode operator~(TrueSkyRenderMode a)
		{
			return static_cast<TrueSkyRenderMode>(~ static_cast<unsigned>(a));
		}
	}
}

namespace std
{
	template<> inline void swap(simul::clouds::KeyframeInUse& _Left, simul::clouds::KeyframeInUse& _Right) SWAP_NOEXCEPT
	{
		std::swap(_Left.size			,_Right.size);
		std::swap(_Left.dens_texel		,_Right.dens_texel);
			
		std::swap(_Left.memoryInterface	,_Right.memoryInterface);	

		std::swap(_Left.lightingSetupStruct	,_Right.lightingSetupStruct);
		std::swap(_Left.sun_direction	,_Right.sun_direction);

		std::swap(_Left.moon_directions	,_Right.moon_directions);
	}
}
namespace simul
{
	namespace clouds
	{
		struct CloudWindow;
		SIMUL_SKY_EXPORT_STRUCT SubdivisionState//:public SubdivisionState
		{
			SubdivisionState(simul::base::MemoryInterface *)
				:checksum(0)
				,keyframe()
			{
			};
			void Reset()
			{
				keyframe.SetChecksum(0);
				checksum=0;
			}
            const CloudKeyframe &GetKeyframe() const
            {
                return keyframe;
            }
            CloudKeyframe &GetKeyframe()
            {
                return keyframe;
            }
			unsigned checksum;
        private:
			CloudKeyframe keyframe;
		};
		/*!
            A class to manage interpolation between cloud states over time.
            Typically, a CloudKeyframer is created and updated by its \link simul::clouds::Environment Environment\endlink.
            
            If time and daytime are linked in the SkyKeyframer, time is in days, otherwise the timescale is in arbitrary units.
        */
		SIMUL_CLOUDS_EXPORT_CLASS CloudKeyframer :
			public simul::sky::OvercastCallback
			, public sky::BaseKeyframer
		{
		public:
			//! Constructor
			CloudKeyframer(simul::base::MemoryInterface *mem=NULL,bool make2d=false);
			//! Destructor
			~CloudKeyframer();
			const CloudKeyframer &operator=(const CloudKeyframer& SK);
			META_BeginProperties
				META_PropertyWithSetCall(float		,TimeFactor					,TimeFactorChanged				,"Time factor for wind etc, the default is 60*60*24, the number of seconds in a keyframe time unit.")
				META_Property(bool					,OverrideWind												,"If true, Wind is set by WindSpeed and WindHeading directly, not through keyframes.")
				META_Property(bool					,OverrideRain												,"If true, Precipitation is set directly, not through keyframes.")
			// Properties for rendering:
				META_RangeProperty(float			,OvercastEffect				,3.f,0.f,50.f					,"How much light is obscured by an overcast cloud layer.")
				META_Property(bool					,Enabled													,"If true, this cloud layer should be rendered.")
				META_Property(float					,Phase														,"Phase for edge noise.")

				META_PropertyWithSetCall(std::string,MapTexture					,SetRecalculate					,"A texture to use as a mask for this layer.")

				META_Property(bool					,ExplicitOffsets											,"If true, the cloud offset (i.e. wind motion) is set as an absolute position, instead of using WindSpeed.")
				META_Property(CloudClass			,Class														,"General type of cloud in this layer.")
				
				META_RangeProperty(float			,PrecipitationThresholdKm	,0.2f,0.f,100.f					,"What local thickness of cloud is needed for rain to fall.")

				META_RangeProperty(float			,VorticityConfinement		,0.0f,0.f,1.f					,"Magnitude of vorticity confinement effect in simulation.")
				META_RangeProperty(float			,Viscosity					,0.0f,1.f,1.f					,"Magnitude of viscosity in simulation.")
				META_Property(std::string			,Name														,"Name of the layer.")
					
			META_EndProperties 
			
			static long long GetEnum(const char *)
			{
				return 0;
			}
			virtual unsigned GetMemoryUsage() const;
			//! \deprecated Apply the cloud's properties to all keyframes.
			//void InitKeyframesFromClouds();
			//! Create a number of evenly spaced keyframes.
			void SetUniformKeyframes(int StepsPerDay,float range=1.f);
			//! Restart all the frame-dependent values (mainly wind offsets), for example in order to synchronize rendering across several channels.
			void Synchronize();
			//! Fills the strikes array and returns how many active strikes in the storm
            //! NOTE: right now we only consider 1 strike.
			int GetExportLightningStrikes(ExportLightningStrike *strikes,int max_s,float game_time,float real_time,const CloudWindow *w) const;
			//! The number of storms on the timeline.
			int GetNumStorms() const
			{
				return (int) storms.size();
			}
			//! Add a storm between the times specified, at the given centre \em c, with horizontal radius r (km).
			Storm *AddStorm(float t0,float t1,crossplatform::Quaterniond origin,float r_km);
			//! Get the storm, if any, that is active at time \em t.
			const Storm *GetStorm(float t) const;
			//! Get the storm, if any, that is active at time \em t.
			Storm *GetStorm(float t);
			//! Return the storm with unique identifier \em uid.
			Storm *GetStormByUniqueId(sky::uid uid);
			//! Return the storm with unique identifier \em uid.
			const Storm *GetStormByUniqueId(sky::uid uid) const;
			//! Return the storm with index \em i.
			Storm *GetStorm(int i);
			//! Remove the storm with index \em i.
			void DeleteStorm(int i);
			//! Remove the storm with unique id \em uid.
			void DeleteStormByUniqueId(sky::uid uid);
			std::vector<Storm*> storms;
			//! Get the properties of the currently active lightning strike
			const LightningProperties &GetLightningProperties(float game_time,float real_time,const CloudWindow *window) const;
			const sky::BaseKeyframe *GetInterpolatedBaseKeyframe() const override
			{
				return &InterpolatedKeyframe;
			}
			//! Get the current \em interpolated keyframe, which holds the values interpolated from the two surrounding keyframes at
			//! any given time.
			CloudKeyframe &GetInterpolatedKeyframe()
			{
				return InterpolatedKeyframe;
			}
			const CloudKeyframe &GetInterpolatedKeyframe() const
			{
				return InterpolatedKeyframe;
			}
			int GetGridHeight() const
			{
				return cloudNode.GetGridHeight();
			}
			int GetGridWidth() const
			{
				return cloudNode.GetGridWidth();
			}
			void SetGridHeight(int h)
			{
				cloudNode.SetGridHeight(h);
			}
			void SetGridWidth(int w) 
			{
				cloudNode.SetGridWidth(w);
			}
			//! Force clouds to regenerate.
			void SetRecalculate();
			//! The next keyframe not partially built or being used. This keyframe's properties can be modified without any pause or rebuild.
			CloudKeyframe *GetNextModifiableKeyframe();
			const sky::BaseKeyframe *GetNextModifiableKeyframe(float time) const override;
			//! Insert a new keyframe at time t, sorting in between the existing keyframes if needed, and return a pointer to it.
			CloudKeyframe *InsertKeyframe(float t) override;
			//! Remove a keyframe
			void DeleteKeyframe(int i);
			//!Remove Keyframe by UID
			void DeleteKeyframeByUID(sky::uid uid);
			//! Number of keyframes.
			virtual int GetNumKeyframes() const override;
			//! Get a pointer to the keyframe with the given ID
			sky::BaseKeyframe *GetKeyframe(int i) override;
			const sky::BaseKeyframe *GetKeyframe(int i) const override;
			void SortKeyframes() override;
			// Implementing OvercastCallback
			sky::OvercastStruct GetOvercastData(double time) const override;
			
			//! Return true if the keyframe has a float value with the given, case-insensitive, name; return false otherwise.
			bool HasFloat(const char *name) const override;
			//! Get a float with the given, case-insensitive, name
			float GetFloat(const char *name,const base::Variant *params=NULL) const override; 
			//! Return the default float value with the given, case-insensitive, name.
			float GetDefaultFloat(const char *name) const override;
			//! Set a float with the given, case-insensitive, name
			void SetFloat(const char *name,float val) override;
			
			//! Return true if the keyframe has an int value with the given, case-insensitive, name; return false otherwise.
			bool HasInt(const char *name) const override;
			//! Get an int with the given, case-insensitive, name
			int GetInt(const char *name,const base::Variant *params=NULL) const override;
			//! Return the default int value with the given, case-insensitive, name.
			int GetDefaultInt(const char *name) const override;
			//! Set an int with the given, case-insensitive, name
			void SetInt(const char *name,int val) override;

			//! Set the properties of a custom cloud volume:
			void SetVolume(sky::uid id,CloudVolumeType type,const crossplatform::Quaterniond &orig,vec2 lscale,vec2 uscale,vec2 rake,vec2 height_range_km);
			//! Remove a custom cloud volume
			void RemoveVolume(sky::uid id);
			std::map<sky::uid,CloudVolume> &GetVolumes();
			const std::map<sky::uid,CloudVolume> &GetVolumes() const;
			//! Set the cloud interface. As the constructor needs a CloudInterface, it is not usually necessary to call this.
			CloudInterface *GetCloudInterface();
			CloudGridInterface *GetCloudGridInterface();
			const CloudInterface *GetCloudInterface() const;
			const CloudGridInterface *GetCloudGridInterface() const;
			
			const simul::clouds::CloudProperties &GetCloudProperties() const;
			simul::clouds::CloudProperties &GetCloudProperties();
			//! Set the sky - this is used to light the clouds over time.
			void SetSkyInterface(simul::sky::BaseSkyInterface *si);
			//! Relocate: to avoid numerical precision problems, relocation can be performed. Specify any position, before and after relocation.
			void Relocate(const float pos_before[3],const float pos_after[3]);
			//! Once-per-frame update for the keyframer, call this with an absolute time value (not a time step).
			void Update(float new_time);
			//! Force recalculation of the per-keyframe tables.
			void Reset();
			vec3 GetEdgeNoiseOffset(float wavelength,float fall_speed,float time) const;
			//! Get the interpolation - between zero and one. This is based on InterpStepTime and the sky's Daytime property.
			//! When the interpolation reaches one, the textures will be cycled.
			float GetInterpolation() const
			{
				return GetSubdivisionInterpolation(InterpolatedKeyframe.time).interpolation;
			}
			//! For debugging purposes only.
			void ForceRelight();
			//! How much precipitation there is at this point. This value is only approximate - it does not account for the actual cloud cover generated on GPU,
			//! so it only gives the maximum precipitation that is possible at this point.
			float GetMaximumLocalPrecipitation(const float pos[3]) const;
			///
			enum GridVariable
			{
				DENSITY=0,BRIGHTNESS,SECONDARY,AMBIENT
			};
			//! Make the wind offsets loop based on the loop time.
			void SetLoopWind();
			//! Save to a binary stream:
			void Save(simul::sky::Output &os) const override;
			//! Load from a binary stream:
			void Load(simul::sky::Input &is) override;
			//! Save to a text file
			void SaveToText(simul::crossplatform::TextOutput &output,bool include_keyframes) const override;
			//! Load from a text file
			void LoadFromText(simul::crossplatform::TextInput &input) override;
			//! Clear keyframes.
			void New();
			//! Check for changed offsets and recalculate.
			void RecalculateOffsets();
			//! Get the offset of the first keyframe, a mutable property determined by Relocate().
			math::Vector3 GetInitialOffset() const;
			//! Get the origin of the render grid; this is adjusted internally when Relocate() is called.
			math::Vector3 GetGridOrigin() const;
			///// REPLACING CALLBACK SEMANTICS WITH ITERATOR MODEL:
			int3 GetTextureSizes();

			void GetGpuCloudsParameters(GpuCloudsParameters&,int index);
			void GetGpuSimulationParameters(GpuCloudsParameters &);
			//! Get the current offset of the cloud window due to wind.
			math::Vector3 GetWindOffsetKm() const;
			sky::BaseKeyframe *GetSubdivisionKeyframe(int index) override;
			const sky::BaseKeyframe *GetSubdivisionKeyframe(int index) const override;
			const sky::BaseKeyframe *GetInterpolationStartKeyframe() const
			{
				return &interpolationStartKeyframe;
			}
			sky::BaseKeyframe *GetInterpolationStartKeyframe()
			{
				return &interpolationStartKeyframe;
			}
			/// Checksum for the interpolated state.
			unsigned GetInterpolationChecksum() const;
			/// This is a checksum that only changes if an in-use subdivision has modified (cycling does not change it).
			unsigned GetSubdivisionChecksum() const;
			virtual unsigned GetGlobalChecksum() const override;
		public:
			float GetPlanetRadius() const;
		protected:
			GpuCloudsParameters gpuCloudsParameters[4];
			std::map<sky::uid,CloudVolume> cloudVolumes;
			void EnsureSubdivisionValidity(int index) override;
			void EnsureUseStateValidity(KeyframeInUse &U,const sky::SubdivInterpolation &s,int index);
			simul::clouds::CellularCloudNode cloudNode;
			// Subdivisions
			void SubdivisionMethodChanged() override;

			simul::base::ShowProgressInterface *FindProgressCallback();

			CloudKeyframe InterpolatedKeyframe;
			CloudKeyframe interpolationStartKeyframe;
			CloudKeyframe interpolationEndKeyframe;
			void FillIlluminationTexture(int i,int texel_index,int num_texels,const unsigned char *uchar8_array);
			void UserKeyframesChanged();
			void InterpKeyframe(CloudKeyframe *K) const;
			void InterpolateKeyframe(CloudKeyframe* K,const CloudKeyframe* K1,const CloudKeyframe* K2,float interp) const;
			void InterpolateKeyframeFromSubdivisions(CloudKeyframe* K) const;
			void NotCompatibleWithUserKeyframes();
			//bool IsTextureCorrectlySetUp(int index);
			void IncrementKeyframe();
			void DecrementKeyframe();
			void CycleTexturesForward() override;
			void CycleTexturesBackward() override;
			//! Checksum to determine whether any keyframe has altered its position due to wind speed or direction.
			unsigned GetOffsetChecksum() const;
			unsigned offset_checksum;
			void CalcOffsets();
			void InitLightDirection(KeyframeInUse &U);
			void InitCallback();
			void EnsureFillTextureSizes();
			void UpdateCloudTexture(int index,float fill_intep);
			void TimeFactorChanged();
			CloudKeyframe					defaultKeyframe;
			CloudKeyframe					emptyKeyframe;
			simul::sky::BaseSkyInterface	*skyInterface;
			KeyframeInUse					keyframesInUse[3];
			 
			float last_time;
			
			unsigned force_recalc;
			unsigned globalChecksum;
			void PartialUpdate(int index);
			
			mutable simul::math::RandomNumberGenerator *random;
			bool IsKeyframeInUse(int k) const override;
			math::Vector3 initialOffset;
			math::Vector3 gridOffset;
			vec2 loopOffsetKm;// how far travelled due to wind in one complete loop.
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif