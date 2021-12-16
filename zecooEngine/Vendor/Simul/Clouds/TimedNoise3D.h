#ifndef SIMUL_MATH_TIMEDNOISE3D_H
#define SIMUL_MATH_TIMEDNOISE3D_H
#include "Platform/Math/Noise3D.h"
#include "Simul/Clouds/Export.h"
namespace simul
{
	namespace clouds
	{
		//! A 3D noise generator used for cloud volumes, where the value returned depends on normalized spatial co-ordinates, and on normalized time.
		SIMUL_CLOUDS_EXPORT_CLASS TimedNoise3D:public math::Noise3D
		{
			float z_max;
			float time;
			float cnoise3(float p[3],float angle,int kmax) const;
			float cnoise2(float p[2],float angle,int freq) const;
			float cvalue_2d_at(int i,int j,float angle,int freq) const;
			float cvalue_at(int i,int j,int k,float angle,int kmax) const;
			float initial_grid_height;
		public:
			TimedNoise3D(simul::base::MemoryInterface *mem=0);
			virtual ~TimedNoise3D();
			void SetInitialGridHeight(float h);
			float PerlinNoise4D(int x,int y,int z,float t) const;
			float PerlinNoise4D(float x,float y,float z,float t) const;
			//! Set the current time value for noise lookups (normalized to have a period of one).
			void SetTime(float t);
		};
	}
}

#endif

