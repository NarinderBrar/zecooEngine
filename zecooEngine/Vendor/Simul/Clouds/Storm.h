#pragma once
#include "Simul/Clouds/Export.h"
#include "Simul/Clouds/CloudInterface.h"
#include "Platform/Math/RandomNumberGenerator.h"
#include "Platform/CrossPlatform/TextInputOutput.h"
#include "Simul/Clouds/LightningRenderInterface.h"
#include "Simul/Sky/BaseKeyframer.h"
#include <vector>
#include <map>

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif
#ifndef SWAP_NOEXCEPT
#define SWAP_NOEXCEPT noexcept
#endif

namespace std
{
	template<> inline void swap(simul::clouds::LightingSetupStruct& _Left, simul::clouds::LightingSetupStruct& _Right) SWAP_NOEXCEPT
	{
		std::swap(_Left.light_axis				,_Right.light_axis				 );
		std::swap(_Left.light_direction			,_Right.light_direction			 );
		std::swap(_Left.light_vector			,_Right.light_vector			 );
		std::swap(_Left.light_scales			,_Right.light_scales			 );
		std::swap(_Left.light_origin			,_Right.light_origin			 );
		std::swap(_Left.light_gridsizes[0]		,_Right.light_gridsizes[0]		 );
		std::swap(_Left.light_gridsizes[1]		,_Right.light_gridsizes[1]		 );
		std::swap(_Left.light_gridsizes[2]		,_Right.light_gridsizes[2]		 );
//		std::swap(_Left.secondary_extinction	,_Right.secondary_extinction	 );
		std::swap(_Left.light_gridsize			,_Right.light_gridsize			 );
		std::swap(_Left.ambient_gridsize		,_Right.ambient_gridsize		 );
		std::swap(_Left.ambient_gridsizes[0]	,_Right.ambient_gridsizes[0]	 );
		std::swap(_Left.ambient_gridsizes[1]	,_Right.ambient_gridsizes[1]	 );
		std::swap(_Left.ambient_gridsizes[2]	,_Right.ambient_gridsizes[2]	 );
		std::swap(_Left.secondary_gridsize		,_Right.secondary_gridsize		 );
		std::swap(_Left.under_light_multiplier	,_Right.under_light_multiplier	 );
	}
}
namespace simul
{
	namespace clouds
	{
		struct CloudWindow;
		//! A structure representing a thunderstorm. See CloudKeyframer::AddStorm.
		SIMUL_CLOUDS_EXPORT_STRUCT Storm
		{
			Storm(math::RandomNumberGenerator *r)
				:random(r)
				,radiusKm(20.f)
				,start_time(0.f)
				,end_time(1.f)
				,realTime(true)
				,sheet(0.1f)
				,strikeFrequencyPerSecond(0.5f)
				,strikeDurationSeconds(0.5f)
				,strikeThicknessMetres(0.03f)
				,roughness(0.2f)
				,motion(0.0f)
				,seed(1)
				,numLevels(2)
				,numBranches(2)
				,branchAngleDegrees(45.f)
				,colour(1.f,0.8f,0.8f)
				,maxRadiance(1000.0f)
				,minPixelWidth(1.5f)
				,unique_id(0xFFFFFFFF)
				,last_strike_uid(1)
			{
			}
			void		SaveToText(crossplatform::TextOutput &output) const;
			void		LoadFromText(crossplatform::TextInput &input);
			//! Return the currently active strike, or return a default strike
			Strike		GetStrike(int i) const;
			//! Spawn a strike at a certain time with the given properties
			int			SpawnStrike(float time,vec3 startpos,vec3 endpos,float magnitude,vec3 colour,bool sh,float dur,float leader_dur);
			//! Get the properties of the currently active lightning strike
			const LightningProperties &GetLightningProperties(float game_time,float real_time,float base_km,const CloudWindow *w) const;
			int			GetLastStrikeIndex(float game_time,float real_time) const;
			//! Get the approximate number of strikes that are generated over a day with the current settings
			int			GetTotalNumStrikes() const;
			float		GetStrikeTime(int n) const;
			float		GetStrikeTime(float game_time,float real_time) const;
            static const int kStormQueryId = 827613; 
			math::RandomNumberGenerator *random;
			crossplatform::Quaterniond origin;		///< Centre of the lightning storm.
			float		radiusKm;					///< Radius of the lightning storm.
			float		start_time;					///< Time the storm commences.
			float		end_time;					///< Time the storm ends.
			bool		realTime;					///< If true, frequency and duration of strikes are in real time, not simulation time.
			float		sheet;						///< What proportion of the strikes are sheet lightning as opposed to ground strikes.
			float		strikeFrequencyPerSecond;	///< In simulated seconds - not necessarily realtime
			float		strikeDurationSeconds;		///< How long, in simulated seconds - not realtime - each strike should last.
			float		strikeThicknessMetres;		///< Thickness of the main trunk.
			float		roughness;					///< How rough/smooth to make the branches.
			float		motion;						///< How much to move/animate during a strike.
			int			seed;						///< Pseudo-random seed.
			int			numLevels;					///< Number of levels of branching.
			int			numBranches;				///< Mean number of branches to generate from each parent.
			float		branchAngleDegrees;			///< Mean angle of branching.
			vec3		colour;						///< Colour of the strikes.
			float		maxRadiance;				///< Maximum radiance of the strikes.
			float		minPixelWidth;				///< Minimum pixel-size for strike rendering.
			sky::uid	unique_id;					///< A non-persistent unique identifier.
			unsigned	last_strike_uid;
			//! Returns true if the keyframe has the named property.
			bool	HasFloat(const char *name) const;
			//! Returns true if the keyframe type has the named integer property.
			bool	HasInt(const char *name) const;
			//! Returns the named property - see HasFloat().
			float	GetFloat(const char *name) const;
			//! Set the named property - see HasFloat().
			void	SetFloat(const char *name,float val);
			//! Returns the named property - see HasInt().
			int		GetInt(const char *name) const;
			//! Set the named property - see HasFloat().
			void	SetInt(const char *name,int val);
			//! Returns the default for the named property - see HasInt().
			int		GetDefaultInt(const char *name) const;
			//! Returns the default for the named property - see HasFloat().
			float	GetDefaultFloat(const char *name) const;
			//! Once per frame update to discard old strikes.
			void	Update(float game_time,float real_time);
			//! Add the next random strike.
			void	AddStrike(float cloudbase_km,float game_time,float real_time);
			//! We can store a stack of non-random strikes. These take precedence over pseudo-randomly generated strikes.
			std::map<int,Strike> strikes;
			//! Current cached lightning
			mutable LightningProperties lightningProperties;
			static LightningProperties defaultLightningProperties;
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif