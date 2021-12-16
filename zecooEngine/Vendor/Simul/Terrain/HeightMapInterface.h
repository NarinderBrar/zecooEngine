#ifndef SIMUL_TERRAIN_HEIGHTMAPINTERFACE_H
#define SIMUL_TERRAIN_HEIGHTMAPINTERFACE_H

#include "Simul/Terrain/Export.h"
#include "Platform/Core/PropertyMacros.h"
#include "Platform/Math/Vector3.h"

namespace simul
{
	namespace terrain
	{
		SIMUL_TERRAIN_EXPORT_CLASS HeightMapInterface
		{
		public:
			VIRTUAL_GetAndSet(int,PageSize						,"How large is a page of tiles (in vertices)? Must be (2^n)+1.")
			VIRTUAL_GetAndSet(int,TileSize						,"How large is each tile? Must be (2^n)+1 and be smaller than PageSize.")
			VIRTUAL_GetAndSet(float,PageWorldX					,"The size of a terrain page, in world units.")
			VIRTUAL_GetAndSet(float,PageWorldY					,"The size of a terrain page, in world units.")
			VIRTUAL_GetAndSet(float,BaseAltitude				,"Minimum height of the terrain.")
			VIRTUAL_GetAndSet(simul::math::Vector3,Origin		,"Position of the terrain origin in space.")
			VIRTUAL_GetAndSet(float,MaxHeight					,"Maximum height of the terrain.")
			VIRTUAL_GetAndSet(int,NumMipMapLevels				,"Upper LOD limit.")
	
			virtual void SetData(const float *,int,int)=0;
			virtual const float *GetData() const=0;
			virtual bool IntersectLineSegment(const float start[],const float end[],float intersection[])=0;
			virtual float GetHeightAt(float x,float y) const=0;
			virtual float GetHeightAt(int x,int y) const=0;
			virtual const float *GetNormalAt(int x,int y) const=0;
			virtual const float *GetNormalAt(float x,float y) const=0;
			virtual void SetHeightAt(int x,int y,float h)=0;
			virtual void GetBlockCorners(int i,int j,float*x1,float *x2) const=0;
			virtual void GetVertexPos(int i,int j,float*x) const=0;
			virtual bool Collide(const float *x1,const float *d,float *i) const=0;
		};
	}
}
#endif // ifndef WORLD_H

