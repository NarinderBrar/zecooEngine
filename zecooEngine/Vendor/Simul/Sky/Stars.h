#pragma once
#include <vector>
#include <set>
#include <string>
#include <map>
#include <math.h>
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Sky/Export.h"
#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif
namespace simul
{
	namespace sky
	{
		struct Star
		{
			double declination;
			double ascension;
			double magnitude;
			char constellation;
		};
		struct Constellation
		{
			std::set<int> starIndices;
		};
		//! A class that stores star positions by declination, ascension and magnitude.
		SIMUL_SKY_EXPORT_CLASS Stars
		{
		public:
			Stars();
			virtual ~Stars();
			static inline vec3 GetDirection(double ra,double de)
			{
				vec3 d;
				d.x=(float)(cos(de)*cos(ra));
				d.y=(float)(cos(de)*sin(ra));
				d.z=(float)sin(de);
				return d;
			}
			//! Initialize the stars with only Ursa Major as seen from Earth. Useful for testing ephemerides.
			void UseUrsaMajor(float max_magnitude);
			//! Initialize the stars with the default stars - the stars seen from Earth.
			void UseDefaultStars();
			void SetMagnitude(float max_magnitude);
			void LoadAsText(const char *filename,float max_magnitude=1e12f);
			void SaveAsText(const char *filename);
			void Load(const char *filename);
			void Save(const char *filename);
			//! Get the total number of stars
			int GetNumStars() const;
			/// Get a star from the currently visible list
			const Star &GetVisibleStar(int i) const;
			//! Get the total number of visible stars
			int GetNumVisibleStars() const;
			/// Get a star from the complete list
			const Star &GetStar(int i) const;
			const Constellation *GetConstellation(const std::string &) const;
			const std::map<std::string,Constellation*> &GetConstellations() const;
		protected:
			float maxMagnitude;
			std::vector<Star> stars;
			std::vector<Star> visibleStars;
			std::map<std::string,Constellation*> constellations;
		};
	}
}

#ifdef _MSC_VER
    #pragma warning(pop)
#endif
