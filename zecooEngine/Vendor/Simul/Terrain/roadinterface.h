#ifndef SIMUL_TERRAIN_ROADINTERFACE_H
#define SIMUL_TERRAIN_ROADINTERFACE_H

#include "Simul/Terrain/Export.h"
#include "Platform/Core/PropertyMacros.h"

namespace simul
{
	namespace terrain
	{
		SIMUL_TERRAIN_EXPORT_CLASS RoadInterface;
		class RoadClientInterface
		{
		public:
			virtual void Rebuild(class RoadInterface *)=0;
			virtual void PartialRebuild(class RoadInterface *,int XMin,int XMax,int YMin,int YMax)=0;
		};
		SIMUL_TERRAIN_EXPORT_CLASS RoadInterface
		{
		public:
			VIRTUAL_GetAndSet(float,Width,"How wide.")
			//! How many spline segments?
			virtual int GetNumSegments() const=0;
			//! How long
			virtual int GetSegmentLength() const=0;
			virtual const float *GetSegmentData() const=0;
			virtual int GetNumSubSegments(int segment) const=0;
			virtual float GetLength() const=0;
			virtual const float *GetCentreLine(float t) const=0;
			virtual const float *GetEdgeOffset(float t) const=0;
		};
	}
}
#endif // ifndef WORLD_H

