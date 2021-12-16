#ifndef SIMUL_FadeTable_H
#define SIMUL_FadeTable_H

#include <vector>
#include <fstream>
#include <set>
#include "Platform/Core/PropertyMacros.h"
#include "Simul/Base/Variant.h"
#include "Platform/Core/Timer.h"
#include "Platform/Core/MemoryInterface.h"
#include "Simul/Base/Referenced.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Sky/Export.h"
#include "Simul/Sky/Float4.h"
#include "Simul/Sky/ChunkInputOutput.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif
namespace simul
{
	namespace sky
	{
		/// An id that is unique for the type of thing it identifies, but not globally unique.
		typedef unsigned long long id;
		/// A globally unique id.
		typedef unsigned long long uid;
		/// Get an unused uid.
		extern SIMUL_SKY_EXPORT uid GenerateUid();
		/// Claims the specified uid so it can't be returned by GenerateUid()
		extern SIMUL_SKY_EXPORT void UseUid(uid u);
		static const uid uid_none = 0;

		struct seq_texture_fill
		{
			seq_texture_fill():texel_index(0),num_texels(0),uint32_array(0),float_array_1(NULL),float_array_2(NULL)
			{}
			void clear()
			{
				num_texels=texel_index;
				uint32_array=NULL;
				float_array_1=float_array_2=float_array_3=NULL;
			}
			int texel_index;
			int num_texels;
			const unsigned *uint32_array;
			const float *float_array_1;
			const float *float_array_2;
			const float *float_array_3;
		};
		struct block_texture_fill
		{
			block_texture_fill(int x=0,int y=0,int z=0,int w=0,int l=0,int d=0
				,const unsigned *uint32_array=0,const float *f1=0,const float *f2=0,const float *f3=0)
			{
				this->x=x;
				this->y=y;
				this->z=z;
				this->w=w;
				this->l=l;
				this->d=d;
				this->uint32_array=uint32_array;
				this->float_array_1=f1;
				this->float_array_2=f2;
				this->float_array_3=f3;
				texel_index=0;
			}
			int texel_index;
			int x,y,z,w,l,d;
			const unsigned *uint32_array;
			const float *float_array_1;
			const float *float_array_2;
			const float *float_array_3;
		};
		//! The base keyframe structure, containing \ref time, the time that the keyframe represents.
		SIMUL_SKY_EXPORT_STRUCT BaseKeyframe
		{
		protected:
			mutable unsigned checksum;
		public:
			BaseKeyframe();
			virtual ~BaseKeyframe(){}
			virtual void ApplyLimits(){}
			virtual unsigned GetPropertiesChecksum() const {return 0;}
			unsigned GetChecksum() const
			{
				return checksum;
			}
			void SetChecksum(int c) const;
			//! The time this keyframe is for.
			float time;
			//! A unique id. This is now persistent across Save/Load.
			uid unique_id;
			//! Save this keyframe's values to the stream.
			virtual void Save(simul::sky::Output &os) const;
			//! Load this keyframe's values from the stream.
			virtual void Load(Input &is,simul::base::MemoryInterface *memoryInterface);
			//! Load from a textual input.
			virtual void LoadFromText(crossplatform::TextInput &input);
			//! Save this keyframe's values to textual output.
			virtual void SaveToText(crossplatform::TextOutput &os) const;
			//! Return true if the keyframe has a float value with the given, case-insensitive, name; return false otherwise.
			virtual bool HasFloat(const char *name) const;
			//! Return true if the keyframe has an int value with the given, case-insensitive, name; return false otherwise.
			virtual bool HasInt(const char *name) const;
			//! Return the float value with the given, case-insensitive, name.
			virtual float GetFloat(const char *name) const;
			//! Set the float value with the given, case-insensitive, name.
			virtual void SetFloat(const char *name,float val);
			//! Set the integer value with the given, case-insensitive, name.
			virtual void SetInt(const char *,int );
			//! Return the integer value with the given, case-insensitive, name.
			virtual int GetInt(const char *) const;
			virtual bool Has(const char *) const{return false;}
			virtual void Set(const char *,const base::Variant *){}
			virtual base::Variant Get(const char *,const base::Variant *){return base::Variant();}
			//! Return the colour value with the given, case-insensitive, name.
			virtual simul::sky::float4 GetColour(const char *) const{return simul::sky::float4();}
			//! Set the colour value with the given, case-insensitive, name.
			virtual void SetColour(const char *,simul::sky::float4 ){}
		};
		/// A structure to represent a position in the subdivisions.
		struct SIMUL_SKY_EXPORT KeyframeInterpolation
		{
			/// Which keyframe we are in.
			int keyframe;
			float interpolation;
			bool isValid() const
			{
				return (interpolation>=0&&interpolation<=1.f);
			}
		};
		/// A structure to represent a position in the subdivisions.
		struct SIMUL_SKY_EXPORT SubdivInterpolation
		{
			/// Which subdivision we are in.
			int subdivision;
			float interpolation;
			double time;
			double real_time;
			bool isValid() const
			{
				return (interpolation>=0&&interpolation<=1.f);
			}
		};
		struct SIMUL_SKY_EXPORT InterpolationInSubdivision
		{
			float interpolation;
			int i0,i1;
		};
		enum SubdivisionRecalculationMode
		{
			RECALC_SUBDIVISIONS_INSTANTLY
			,RECALC_SUBDIVISIONS_GRADUALLY
		};
		/// How to subdivide the keyframes for GPU generation.
		enum SubdivMethod
		{
			FIXED_NUMBER=0,FIXED_GAMETIME=1,FIXED_REALTIME=2,NUM_METHODS
		};
		//! The base class for keyframers. A keyframer stores a list of keyframes, each representing the state at
		//! a particular time.
		SIMUL_SKY_EXPORT_CLASS BaseKeyframer
		{
		public:
			BaseKeyframer(simul::base::MemoryInterface *mem);
			virtual ~BaseKeyframer();
			META_BeginProperties
				META_Property(bool										,DayLoop																							,"Whether to loop keyframes - if true, LoopRange determines the looping point.")
				META_Property(float										,LoopStart																							,"The start time for looping, defaults to zero.")
				META_Property(float										,LoopRange																							,"The timescale for keyframe looping. All kefyrames should have times between 0 and this range.")
				META_PropertyWithSetCall(SubdivisionRecalculationMode	,RecalculationMode,RecalculationModeChanged															,"The mode that determines whether, when keyframes properties change, the subdivisions in use are automatically recalculated.")
				META_RangePropertyWithSetCall(SubdivMethod				,SubdivisionMethod							,FIXED_NUMBER,FIXED_REALTIME,SubdivisionMethodChanged	,"How keyframe intervals are divided up for interpolation.")
				META_RangePropertyWithSetCall(int						,NumSubdivisions							,1,128						,SubdivisionMethodChanged	,"Number of subdivisions between keyframes")
				META_RangePropertyWithSetCall(float						,SubdivisionTimeInterval					,0.000001f,0.1f				,SubdivisionMethodChanged	,"Subdivisions to be spaced by a given fraction of a day.")
				META_RangeProperty(float								,SubdivisionRealTimeInterval				,1.0f,0.0000001f,1000.f									,"Subdivisions to be spaced by a given number of seconds.")
			META_EndProperties
				
			// Set the "real time" value in seconds.
			void SetRealTime(double t);
			void Update(bool modified[],float time,sky::SubdivInterpolation &subdivStruct);
			virtual const BaseKeyframer &operator=(const BaseKeyframer& aft);
			typedef std::vector<BaseKeyframe*> KeyframeVector;
			const KeyframeVector &GetKeyframes() const {return keyframes;}
			KeyframeVector &GetKeyframes() {return keyframes;}
			//! Text-based interface - returns true if the keyframer has a floating point value of the given name.
			virtual bool HasFloat(const char *) const;
			//! Text-based interface - returns the named value if it exists.
			virtual float GetFloat(const char *,const base::Variant *params=NULL) const;
			
			//! Text-based interface - returns true if the keyframer has a value of the given name.
			virtual bool HasInt(const char *) const;
			//! Text-based interface - returns the named value if it exists.
			virtual int GetInt(const char *,const base::Variant *params=NULL) const;
			virtual void SetInt(const char *name,int val);
			virtual void Set(const char *name, const base::Variant *params);
			virtual const BaseKeyframe *GetNextModifiableKeyframe(float time) const;

			//! Return the default float value with the given, case-insensitive, name.
			virtual float GetDefaultFloat(const char *name) const=0;
			//! Return the default int value with the given, case-insensitive, name.
			virtual int GetDefaultInt(const char *name) const=0;
			//!Text-based interface - sets the named value if it exists.
			virtual void SetFloat(const char *,float ){}
			//! Get an index for the keyframe with the specified id, or NULL if there is no such keyframe.
			int GetKeyframeIndexByUniqueId(sky::uid id) const;
			//! Get a pointer to the keyframe with the specified id, or NULL if there is no such keyframe.
			BaseKeyframe *GetKeyframeByUniqueId(sky::uid id);
			//! Get the keyframe uid if present
			sky::uid GetKeyframeUid(BaseKeyframe *k) const;
			//! Get a pointer to the keyframe with the specified id, or NULL if there is no such keyframe.
			const BaseKeyframe *GetKeyframeByUniqueId(sky::uid id) const;
			//! Delete the keyframe if it exists (return true), else return false;
			bool DeleteKeyframeByUniqueId(sky::uid id);
			//! Get a pointer to the i'th keyframe.
			virtual BaseKeyframe *GetKeyframe(int i);
			//! Get a pointer to the i'th keyframe.
			virtual const BaseKeyframe *GetKeyframe(int i) const;
			//! Insert a keyframe at the given time and return a pointer to it.
			virtual BaseKeyframe *InsertKeyframe(float time)=0;
			//! Check if this keyframe is too close to any others. This is done by seeing if it's closer than time_scale.
			//! If so, the specified keyframe is moved away.
			void BumpKeyframe(BaseKeyframe *K,float time_scale);
			//! Delete the specified keyframe.
			void DeleteKeyframe(int i);
			//! Make sure the keyframes are in ascending order of \a time.
			virtual void SortKeyframes();
			//! Number of keyframes.
			virtual int GetNumKeyframes() const;
			//! Time of a particular keyframe.
			virtual float GetKeyframeTime(int i) const;
			//! Returns the index of the current keyframe.
			virtual int GetCurrentSubdivision() const
			{
				return current_subdivision;
			}
			sky::uid GetNearestKeyframeUidToTime(double t,double max_range=1.0) const;
			//! Get the keyframe BEFORE or AT time t:
			virtual int GetKeyframeAtTime(double t) const;
			//! Get the keyframe uid BEFORE or AT time t:
			sky::uid GetKeyframeUidAtOrBeforeTime(double t) const;
			//! Get the keyframe After time t:
			sky::uid GetKeyframeUidAfterTime(double t) const;
			//! Get a list of all the keyframes not currently among the three in-use.
			std::vector<BaseKeyframe*> GetAllModifiableKeyframes();
			//! Stream/save  to the std::osteam os.
			virtual void Save(Output &os) const;
			//! Stream/load from the std::isteam is.
			virtual void Load(Input &is);
			//! Load from a textual input.
			virtual void LoadFromText(crossplatform::TextInput &input);
			//! Save this keyframer's values to textual output.
			virtual void SaveToText(crossplatform::TextOutput &os,bool include_keyframes) const;
			// Test whether the keyframes at the specified time have changed and must be invalidated.
		//	bool ChecksumsFailed();
			unsigned CalcChecksum(BaseKeyframe *K) const;
			virtual void KeyframesChanged(){}
			int GetTextureCycle() const
			{
				return texture_cycle;
			}
			void ResetChecksums();
			int texture_cycle;
			virtual KeyframeInterpolation GetKeyframeInterpolation(double t) const;
			static float FractionalPart(double t,double start,double range)
			{
				t-=start;
				t/=range;
				int T=(int)t;
				if(t<0.f)
					T--;
				double u=(t-(double)T)*range;
				u+=start;
				return (float)u;
			}
			virtual unsigned GetGlobalChecksum() const
			{
				return 0;
			}
			KeyframeVector keyframes;
			void ClearKeyframes();
			bool sorted;
			simul::base::MemoryInterface *memoryInterface;
			virtual const BaseKeyframe *GetInterpolatedBaseKeyframe() const=0;
			virtual const BaseKeyframe *GetSubdivisionKeyframe(int index) const=0;
			virtual BaseKeyframe *GetSubdivisionKeyframe(int index)=0;
			SubdivInterpolation	GetSubdivisionInterpolation(double t) const;
			SubdivInterpolation GetSubdivision(int index,double t) const;
			InterpolationInSubdivision GetInterpolationInSubdivision(float time) const;
		protected:
			void							CalcRealtimeSubdivisionTimes();
			virtual void					CycleTexturesForward()=0;
			virtual void					CycleTexturesBackward()=0;
			mutable std::map<int,double>	realTimeSubdivisionTimes;
			virtual void					EnsureSubdivisionValidity(int i)=0;
			virtual void					SubdivisionMethodChanged(){};
			virtual bool					IsKeyframeInUse(int k) const=0;
			public:
			mutable int current_subdivision;
			mutable double current_subdivision_time;
			mutable double next_subdivision_time;
			int subdivision_cycle_start;
			virtual void					RecalculationModeChanged(){}
			double now_real_time;
			double prev_real_time;
			mutable unsigned subdivision_checksum;
			mutable SubdivInterpolation cachedSubdivInterpolation;
			mutable double cached_epoch_time;
			mutable unsigned cached_subdivision_checksum;
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif