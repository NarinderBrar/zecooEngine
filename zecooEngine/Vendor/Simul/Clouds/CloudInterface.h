#ifndef SIMUL_GRAPH_CLOUDS_CLOUDINTERFACE_H
#define SIMUL_GRAPH_CLOUDS_CLOUDINTERFACE_H
#include "Platform/Math/Vector3.h"
#include "Simul/Clouds/Export.h"
#include "Platform/Core/PropertyMacros.h"

namespace simul
{
	namespace math
	{
		class NoiseInterface;
	}
	namespace clouds
	{
		enum class CloudClass
		{
			STANDARD = 0
			, CIRRUS = 1
			, CUMULUS = 2
			, CUMULONIMBUS=3
			, COMPATIBILITY_41a=4
			, JAMESWIP=5
			, CELLNOISE = 6
		};
		struct LightingSetupStruct
		{
			int light_axis;
			simul::math::Vector3 light_direction;
			simul::math::Vector3 light_vector;
			simul::math::Vector3 light_scales;
			simul::math::Vector3 light_origin;
			int light_gridsizes[3];

			int light_gridsize;
			int ambient_gridsize;
			int ambient_gridsizes[3];
			unsigned secondary_gridsize;
			float under_light_multiplier;
			float cloud_base_z,width,height;
		};
		struct Bits
		{
			unsigned density;
			unsigned brightness;
			unsigned secondary;
			unsigned ambient;
			unsigned Bytes_per_texel;
			bool bigendian;
		};
		class HumidityCallbackInterface;

		/// A virtual interface that returns the keyframe-independent cloud properties. 
		SIMUL_CLOUDS_EXPORT_CLASS CloudProperties
		{
		public:

			VIRTUAL_GetAndSet(bool		,Wrap							,"Wrap the cloud data horizontally")

			VIRTUAL_GetAndSet(unsigned	,RandomSeed						,"Integer random number seed for cloud generation")
			VIRTUAL_GetAndSet(int		,NoiseResolution				,"Resolution of the 3D Perlin noise used to generate clouds")
			VIRTUAL_GetAndSet(float		,NoisePersistence				,"Persistence over successive fractal scales of the cloud-generation noise function. A cloud keyframer will override this setting; see simul::clouds::CloudKeyframe::persistence")
			VIRTUAL_GetAndSet(float		,NoisePeriod					,"Period (in daytime units) over which noise changes")
			VIRTUAL_GetAndSet(float		,NoisePhase						,"Phase (in daytime units) an offset to start of noise function")

			VIRTUAL_GetAndSet(float		,OpticalDensity					,"Density of clouds for rendering. Not used by all renderers.")

			VIRTUAL_GetAndSet(float		,MinimumLightElevationDegrees	,"Minimum elevation for lighting - if light direction is below this angle, it will be raised - useful for sunsets.")

			//! get the effective origin in world co-ordinates, including wind offsets:
			virtual  simul::math::Vector3 GetOrigin(math::Vector3,float cloud_base_z,float w) const=0;
			//! A number that is changed every time the clouds are regenerated.
			virtual int GetGenerationNumber() const=0;
			//! Get the physical extents of the cloud volume in the x, y and z directions.
			virtual void GetExtents(simul::math::Vector3 &X1,simul::math::Vector3 &X2,float cloud_base_z,float w,float h) const=0;

			//! Get a 4x4 matrix to transform from real space to lightspace.
			virtual const float *GetTransformToLightspaceMatrix(unsigned axis,const simul::math::Vector3 &dir,const simul::math::Vector3 &orig,const simul::math::Vector3 &scales) const=0;
			//! Get a 4x4 matrix to transform from lightspace to cloudspace.
			virtual const float *GetTransformLightspaceToCloudspaceMatrix(unsigned axis,const simul::math::Vector3 &dir,const simul::math::Vector3 &orig,const simul::math::Vector3 &scales,float cloud_base_z,float w,float h) const=0;
			virtual const simul::math::Vector3 &ConvertTextureCoordsToReal(const LightingSetupStruct &setup,float w,float h,float x,float y,float z) const=0;
		};
		//! The virtual interface for controlling and reading cloud data.
		SIMUL_CLOUDS_EXPORT_CLASS CloudInterface
		{
		public:
			virtual ~CloudInterface(){}
			//! At the 3D Position \a X, find the optical density of cloud in the direction \a dir.
			virtual float GetOpticalPathLength(float cloud_base_z,float w,float h,const simul::math::Vector3 &X,const simul::math::Vector3 &dir,float time,float cloudiness,float extinction,float diffusivity) const =0;
			virtual float GetOpticalPathLengthBetween(const simul::math::Vector3 &X1,const simul::math::Vector3 &X2,float time,float cloud_base_z,float width,float height,float cloudiness,float extinction,float diffusivity) const =0;
			//! Get the normalized cloud density at real-world position X.
			virtual float GetCloudDensity(float cloud_base_z,float width,float height,const simul::math::Vector3 &X,float time,float cloudiness,float extinction,float diffusivity) const =0;
			//! Get the noise object.
			virtual simul::math::NoiseInterface *GetNoiseInterface()=0;
		};

		//! The virtual interface for controlling and reading a grid of cloud data.
		SIMUL_CLOUDS_EXPORT_CLASS CloudGridInterface
		{
		public:
			VIRTUAL_GetAndSet(int		,GridLength					,"Y (North-South) axis grid size of volumetric texture.")
			VIRTUAL_GetAndSet(int		,GridWidth					,"X (East-West) axis grid size of volumetric texture.")
			VIRTUAL_GetAndSet(int		,GridHeight					,"Z (Up-Down) axis grid size of volumetric texture.")
			//! Get the grid width (x size), length (y size) and height (z size).
			virtual void GetGrid(int &width,int &length,int &height) const =0;
			//! Set the grid width (x size), length (y size) and height (z size).
			virtual void SetGrid(int width,int length,int height) =0;
		};
	}
}

#endif
