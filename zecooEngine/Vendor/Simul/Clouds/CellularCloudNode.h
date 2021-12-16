#ifndef SIMUL_GRAPH_CLOUDS_CELLULARCLOUDNODE_H
#define SIMUL_GRAPH_CLOUDS_CELLULARCLOUDNODE_H

#include "Simul/Base/Referenced.h"
#include "Simul/Clouds/CellularCloudGrid.h"
#include "Platform/Core/MemoryInterface.h"

#include "Platform/Math/Vector3.h"
#ifdef _MSC_VER
	
	#pragma warning(push)
	// C4251 can be ignored in Visual C++ if you are deriving from a type in the Standard C++ Library
	#pragma warning(disable:4251)
	#pragma warning(disable:4541)
#endif

namespace simul
{
	namespace math
	{
		class Vector3;
	}
}

namespace simul
{
	namespace clouds
	{
		//! A callback class to control the distribution of humidity in a cloud node. The value returned by
		//! GetHumidityMultiplier is multiplied by the calculated cloud density.
		//! Derive your own class from HumidityCallbackInterface and implement GetHumidityMultiplier,
		//! then call \link simul::clouds::FastCloudNode::AddHumidityCallback AddHumidityCallback \endlink
		//! to attach it to the cloud node.
		class HumidityCallbackInterface
		{
		public:
			//! The function that returns how dense clouds should be at the stated position, where x, y and z are between
			//! zero and one in the cloud volume.
			virtual float GetHumidityMultiplier(float x,float y,float z) const=0;
		};
		class TimedNoise3D;
	}
}
namespace simul
{
	namespace clouds
	{
		//! A cloud class based on cellular automata.

		//! Don't create a CellularCloudNode directly, instead, use the derived class FastCloudNode.
		SIMUL_CLOUDS_EXPORT_CLASS CellularCloudNode:public CellularCloudGrid
		{
		public:
			CellularCloudNode(simul::base::MemoryInterface *mem=NULL);
			virtual ~CellularCloudNode();
		public:
			META_BeginProperties
			META_Category(Generation)
				META_RangePropertyWithSetCall(float	,NoisePersistence			,0.f	,1.f	,Recalculate		,"Persistence over successive fractal scales of the cloud-generation noise function.")
			META_EndCategory(Generation)
			META_Category(Lighting)
				META_RangePropertyWithSetCall(float	,MinimumLightElevationDegrees,-90.f	,90.f	,NeedRelight		,"Minimum elevation for lighting , default 3 degrees - if light direction is below this angle, it will be raised - useful for sunsets. Used to prevent clouds from being lit from below - if the incoming light angle is below this threshold, it is raised. Although clouds can be lit from below in reality, usually this is only possible for very high altitude clouds, due to occlusion from the Earth.");
				META_Property(bool						,UseCUDA														,"Whether to use CUDA (if available, PC only).");
			META_EndCategory(Lighting)
			META_Category(Appearance)
				META_RangePropertyWithSetCall(float	,OpticalDensity				,0	,12.f		,NeedRender			,"Density of clouds for rendering")
			META_EndCategory(Appearance)
			META_Category(Animation)
				META_RangeProperty(			float	,NoisePeriod				,0		,0		,1e23f				,"Period (in daytime units, one day = one unit) over which noise changes")
				META_RangeProperty(			float	,NoisePhase					,0		,0		,2.0f				,"Phase (in daytime units, one day = one unit) an offset to start of noise function")
				META_Property(simul::math::Vector3	,WindOffset														,"3D wind offset, in metres.")
			META_EndCategory(Animation)
			META_EndProperties
		public:
			//! Get a 4x4 matrix to transform from real space to lightspace.
			const float *GetTransformToLightspaceMatrix(unsigned axis,const math::Vector3 &dir,const math::Vector3 &origin,const math::Vector3 &scales) const;
			//! Get a 4x4 matrix to transform from lightspace to cloudspace.
			const float *GetTransformLightspaceToCloudspaceMatrix(unsigned axis,const math::Vector3 &dir,const math::Vector3 &origin,const math::Vector3 &scales,float cloud_base_z,float w,float h) const;
			//! Get a pointer to the noise memory interface
			simul::math::NoiseInterface *GetNoiseInterface();
			//! Recalculate and reinitialise the cloud grid with updated values
			void SetRecalculate(bool r)
			{
				changed = r;
			}
			//! Force relighting of the volume. Usually unnecessary.
			void SetRelight();

			//! Have properties changed that cause relighting to be necessary - either the light has moved
			//! or the cloud density grid has changed.
			virtual bool NeedsRelight() const
			{
				return GetRelightChecksum()!=relit_checksum;
			}

			virtual void SaveGrid(std::ostream &os) const;
			virtual void LoadGrid(std::istream &is);

		// These are inherited from CloudInterface	
			virtual bool Generate();
			virtual int GetGenerationNumber() const;
			virtual float GetOpticalPathLength(float cloud_base_z,float w,float h,const simul::math::Vector3 &X,const simul::math::Vector3 &dir,float time,float cloudiness,float extinction,float diffusivity) const;
			virtual float GetOpticalPathLengthBetween(const simul::math::Vector3 &X1,const simul::math::Vector3 &X2,float time,float cloud_base_z,float width,float height,float cloudiness,float extinction,float diffusivity) const;
			virtual float GetAmbient(const LightingSetupStruct &setup,float w,float h,const simul::math::Vector3 &X,const float *ambient_grid);
			virtual float GetLight(const LightingSetupStruct &setup,const simul::math::Vector3 &X,const float *light_grid) const;
			virtual float GetSecondaryLight(const LightingSetupStruct &setup,const simul::math::Vector3 &X,const float *light_grid) const;
			
			//! Get the physical extents of the cloud volume in the x, y and z directions.
			virtual void GetExtents(simul::math::Vector3 &X1,simul::math::Vector3 &X2,float cloud_base_z,float w,float h) const;
			//! Get the effective origin in world co-ordinates, including wind offsets:
			virtual  simul::math::Vector3 GetOrigin(math::Vector3 windOffset,float cloud_base_z,float w) const;
			virtual float GetCloudDensity(float cloud_base_z,float width,float height,const simul::math::Vector3 &X,float time,float cloudiness,float extinction,float diffusivity) const;
			virtual void GetGrid(int &w,int &l,int &h) const;
			virtual void SetGrid(int w,int l,int h);
			virtual void GetGridValues(const LightingSetupStruct &setup,float &density,float &light,float &ambient,
										float &second,int x,int y,int z,const float *light_grid,const float *ambient_grid,float time,float cloudiness,float extinction,float diffusivity) const;

			LightingSetupStruct CalcLightingSetup(const simul::math::Vector3 &dir_to_sun,float cloud_base_z,float width,float height) const;
			virtual bool ReLightPartial(const LightingSetupStruct &setup,float w,float h,float proportion,float *light_grid,float *ambient_grid,int &lighting_index,float time,float cloudiness,float extinction);

		//! Stream/save to the std::osteam os.
			std::ostream &StreamOut(std::ostream &os) const;
		//! Stream/load from the std::isteam is.
			std::istream &StreamIn(std::istream &is);
		//! Stream/save to the std::osteam os.
			std::ostream &StateStreamOut(std::ostream &os) const;
		//! Stream/load from the std::isteam is.
			std::istream &StateStreamIn(std::istream &is);

		// inherited from MemoryUsageInterface
			virtual unsigned GetMemoryUsage() const;
			
			const simul::math::Vector3 &ConvertTextureCoordsToReal(const LightingSetupStruct &setup,float w,float h,float x,float y,float z) const;
			const simul::math::Vector3 &ConvertLightCoordsToReal(const LightingSetupStruct &setup,float x,float y,float z) const;
		protected:
			unsigned GetRelightChecksum() const;
			unsigned relit_checksum;
			float lit_humidity;
			float coverage;
			int GenerationNumber;
			virtual void PrepareToDestroy();
			virtual void ChangedNoise();
			void GetGridCoordsAndInterps(const LightingSetupStruct &setup,int &grid_x0,int &grid_y0,int &grid_z0,
										float &interp_x,float &interp_y,float &interp_z,
										const simul::math::Vector3 &X, const simul::math::Vector3 &dir,
										unsigned axis, const int g[3]) const;
			float InterpBooleanGridValues(int grid_x0,int grid_y0,int grid_z0,
											float interp_x,float interp_y,float interp_z) const;
			float InterpGridValues(const LightingSetupStruct &setup,int grid_x0,int grid_y0,int grid_z0,
											float interp_x,float interp_y,float interp_z,
											const float *lgrid,const int *lgridsize) const;
			unsigned regen_checksum;
			std::string filename;
			TimedNoise3D *timed_noise;
			void CalcLightParameters(	const simul::math::Vector3 &light
										,int &axis
										,math::Vector3 &light_vec
										,math::Vector3 &origin
										,math::Vector3 &scales
												,float &under_light_multiplier,float cloud_base_z,float w,float h) const;

			simul::math::Vector3 ProjectToEdge(const simul::math::Vector3 &X0,
				const simul::math::Vector3 &D, const simul::math::Vector3 &x1,
				const simul::math::Vector3 &x2);
			float InternalCalcOpticalPathLength(float cloud_base_z,float w,float h,const simul::math::Vector3 &X,
				const simul::math::Vector3 &dir,float time,float cloudiness,float extinction,float diffusivity) const;
			void GetCoordsFromPosition(const LightingSetupStruct &setup,const simul::math::Vector3 &X, unsigned &x,
			unsigned &y, unsigned &z) const;
			void NeedRelight();
			void NeedRender();
			void ChangedSec();
			// virtual from grid;
			virtual void Recalculate();
			virtual void ChangedGrid();

			void QuicklightChanged();
			void ChangedCacheNoise();
			void *access_mutex;
			simul::base::MemoryInterface *memoryInterface;
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#endif
