#ifndef SIMUL_TERRAIN_HEIGHTMAPNODE_H
#define SIMUL_TERRAIN_HEIGHTMAPNODE_H
#include "Simul/Terrain/HeightMapInterface.h"

#include "Export.h"
#include <string>
#include <vector>
#include "Platform/Math/Vector3.h"
#include "Platform/Core/MemoryInterface.h"
#ifdef _MSC_VER
	
	#pragma warning(push)
	// C4251 can be ignored in Visual C++ if you are deriving from a type in the Standard C++ Library
	#pragma warning(disable:4251)
	#pragma warning(disable:4541)
#endif
namespace simul
{
	namespace terrain
	{
		SIMUL_TERRAIN_EXPORT_STRUCT Filter
		{
			virtual const char *GetType() const=0;
			virtual ~Filter(){}
			std::vector<float> remap_heights;
		};
		SIMUL_TERRAIN_EXPORT_STRUCT RidgeFilter:public Filter
		{
			virtual const char *GetType() const {return "ridge";}
		};
		SIMUL_TERRAIN_EXPORT_CLASS HeightMapNode
			:public HeightMapInterface
		{
		public:
			HeightMapNode(simul::base::MemoryInterface *m=NULL);
			virtual ~HeightMapNode();
			void Rebuild() const;
			META_BeginProperties
				META_PropertyWithSetCall(int	,PageSize			,ChangedWorldSize	,"How large is a page of tiles (in vertices)? Must be (2^n)+1.")
				META_PropertyWithSetCall(int	,TileSize			,ChangedWorldSize	,"How large is each tile? Must be (2^n)+1 and be smaller than PageSize.")
				META_PropertyWithSetCall(float	,PageWorldX			,ChangedWorldSize	,"The size of a terrain page, in world units.")
				META_PropertyWithSetCall(float	,PageWorldY			,ChangedWorldSize	,"The size of a terrain page, in world units.")
				META_PropertyWithSetCall(float	,BaseAltitude		,ChangedWorldSize	,"The base altitude for the terrain.")
				META_PropertyWithSetCall(simul::math::Vector3	,Origin,ChangedWorldSize	,"The world origin position for the terrain.")
				META_PropertyWithSetCall(float	,FlattenBelow		,ChangedWorldSize	,"Level off values below here.")
				META_PropertyWithSetCall(float	,MaxHeight			,ChangedWorldSize	,"Maximum height of the terrain.")
				META_PropertyWithSetCall(float	,FractalScale		,ChangedWorldSize	,"Scale length for fractal terrain.")
				META_PropertyWithSetCall(int	,FractalOctaves		,ChangedWorldSize	,".")
				META_PropertyWithSetCall(int	,FractalFrequency	,ChangedWorldSize	,".")
				META_PropertyWithSetCall(int	,InitialFrequency	,ChangedWorldSize	,".")
				META_PropertyWithSetCall(float	,Persistence		,ChangedWorldSize	,".")
				META_PropertyWithSetCall(int	,NumMipMapLevels	,ChangedWorldSize	,"Upper LOD limit.")

				META_Property(float				,TalusAngleDegrees		,"The talus angle.")
				META_RangeProperty(float		,Rainfall,0.1f,0.f,1.f	,"The amount of yearly rainfall.")
				META_RangeProperty(float		,Evaporation,0.1f,0.f,1.f	,"The amount of evaporation.")
				META_PropertyReadOnly(float		,MinAltitude		,"The actual smallest altitude.")
				META_PropertyReadOnly(float		,MaxAltitude		,"The actual largest altitude.")

			META_EndProperties
			
			
			//! Add (index=-1 or larger than filter count) or insert a filter that will modify the heightmap.
			void AddFilter(int index,const char *type);
			//! Remove the specified filter.
			void RemoveFilter(int index);
			//! The number of filters.
			int GetNumFilters() const;
			//! Get a pointer to the filter at the index i.
			Filter *GetFilter(int i);
			//! Get a pointer to the filter at the index i.
			const Filter *GetFilter(int i) const;

			virtual bool IntersectLineSegment(const float start[],const float end[],float intersection[]);
			virtual float GetHeightAt(float x,float y) const;
			virtual float GetHeightAt(int x,int y) const;
			virtual const float *GetNormalAt(int x,int y) const;
			virtual const float *GetNormalAt(float x,float y) const;
			virtual void SetHeightAt(int x,int y,float h);
			virtual void GetBlockCorners(int i,int j,float*x1,float *x2) const;
			virtual void GetVertexPos(int i,int j,float*x) const;
			virtual bool Collide(const float *x1,const float *d,float *i) const;
			//! Set the heights.
			virtual void SetData(const float *data,int width,int stride);
			//! Get the actual heights in the grid.
			virtual const float *GetData() const;
			//! Checksum to say that any part of the heightmap has changed
			virtual unsigned GetChecksum() const;
			//! Checksum for what rows have changed
			virtual unsigned GetRowChecksum(int row) const;
			//! Checksum for what columns have changed
			virtual unsigned GetColumnChecksum(int col) const;

			const float *GetCentre() const;
		protected:
			simul::base::MemoryInterface *memoryInterface;
			std::vector<Filter*> filters;
			mutable float HeightRange;
			void NeedRebuild();
			void RecalcTiling();
			void ChangedWorldSize();
			mutable float *data;
			void ToGrid(int &X,int &Y,float x,float y) const;
			void ToGrid(int &X,int &Y,float &px,float &py,float x,float y) const;
			void Smooth(float strength);
			mutable bool need_rebuild;
			unsigned overall_checksum;
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif // ifndef WORLD_H

