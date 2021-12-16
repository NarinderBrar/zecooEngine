#ifndef SIMUL_GRAPH_CLOUDS_CELLULARCLOUDGRID_H
#define SIMUL_GRAPH_CLOUDS_CELLULARCLOUDGRID_H
#include <vector>
#ifdef _MSC_VER

#pragma warning(push)
#pragma warning(disable:4251)		// disable dllexport warning for typedefs derived from stl classes!
#endif

#include "Simul/Clouds/TimedNoise3D.h"
#include "Platform/Math/RandomNumberGenerator.h"
#include "Platform/Core/MemoryUsageInterface.h"
#include "Simul/Clouds/Export.h"
#include "Simul/Clouds/CloudInterface.h"


namespace simul
{
		namespace base
		{
			class ShowProgressInterface;
			class MemoryInterface;
		}
	namespace clouds
	{
		//! A cellular automata grid of cloud densities.
		//! Don't create a CellularCloudGrid directly, instead, create a FastCloudNode.
		SIMUL_CLOUDS_EXPORT_CLASS CellularCloudGrid
			:public simul::base::MemoryUsageInterface
			,public CloudProperties
		{
		public:
			CellularCloudGrid();
			virtual ~CellularCloudGrid();
			META_BeginProperties
					META_RangePropertyWithSetCall(int		,GridLength		,8		,1024	,ChangedGrid,"Y (North-South) axis grid size of volumetric texture")
					META_RangePropertyWithSetCall(int		,GridWidth		,8		,1024	,ChangedGrid,"X (East-West) axis grid size of volumetric texture")
					META_RangePropertyWithSetCall(int		,GridHeight		,1		,64		,ChangedGrid,"Z (Up-Down) axis grid size of volumetric texture")
			
					META_RangePropertyWithSetCall(unsigned	,RandomSeed		,1		,1000	,NeedRecalculate,"Integer random number seed for cloud generation")
					META_PropertyWithSetCall(bool			,Wrap							,NeedRecalculate,"Wrap the cloud data horizontally")
					META_RangePropertyWithSetCall(int		,NoiseResolution,4		,128	,ChangedNoise	,"Resolution of the 3D Perlin noise used to generate clouds")
			META_EndProperties
			virtual float GetCloud(int x,int y,int z,float time,float cloudiness,float extinction,float diffusivity) const;
			virtual float InterpCloud(float x,float y,float z,float time,float cloudiness,float extinction,float diffusivity) const;
			void SetNoise(simul::clouds::TimedNoise3D *ni);
			void Init(unsigned l=0,unsigned w=0,unsigned h=0);
			virtual void SaveGrid(std::ostream &os) const;
			virtual void LoadGrid(std::istream &is);
		// MemoryUsageInterface
			virtual unsigned GetMemoryUsage() const;
#ifndef DOXYGEN
			friend class ApplyInit;
#endif
			bool CheckLicense() const;
			simul::clouds::TimedNoise3D *GetNoise() {return noise;}
		protected:
			unsigned GetRebuildChecksum() const;
			simul::clouds::TimedNoise3D *noise;
			bool changed;
			//unsigned *cld;		// cloud density grid - 1 bit per cell, so x*y*z/8 bytes.
			virtual void Recalculate()=0;
			virtual void ChangedGrid();
			virtual void ChangedNoise();
			void Recalc()
			{
				Recalculate();
			}
			void NeedRecalculate();
			void GetIndexAndBit(unsigned x,unsigned y,unsigned z,unsigned &index,unsigned &b) const;
			void SetCloud(unsigned x,unsigned y,unsigned z);
			unsigned gridsize;	// number of 4-byte numbers in grid
			unsigned license_code[2];
			float noiseInterp;
			bool noise_changed;
			bool enabled;
		};
	}
}

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#endif
