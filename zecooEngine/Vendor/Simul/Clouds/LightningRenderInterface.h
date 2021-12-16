#ifndef SIMUL_GRAPH_CLOUDS_LIGHTNINGRENDERINTERFACE_H
#define SIMUL_GRAPH_CLOUDS_LIGHTNINGRENDERINTERFACE_H
#include "Simul/Clouds/Export.h"
#include "Platform/Core/PropertyMacros.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/lightning_constants.sl"

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif


namespace simul
{
	namespace math
	{
		class Vector3;
	}
	namespace sky
	{
		struct float4;
	}
	namespace clouds
	{
		struct Storm;
		struct SIMUL_CLOUDS_EXPORT Strike
		{
			int		seed;
            //! Strike start position in metres 
			vec3	startX;
            //! Strike end position in metres
			vec3	endX;
			float	strikeStartTime;
			float	strikeDuration;
			float	leaderDuration;
            //! If true we will get an in-cloud strike, otherwise, cloud to ground (alt=0m) strike
			bool	sheet;
			float   magnitude;
			vec3    colour;
		};
		struct SIMUL_CLOUDS_EXPORT StrikeState
		{
			float	progress;
			float	brightness;
		};
		/// A struct for passing lighting strike properties between trueSKY and an external engine.
		struct SIMUL_CLOUDS_EXPORT ExportLightningStrike 
		{
			int id;
			vec3 pos;
			vec3 endpos;
			float brightness;
			vec3 colour;
			int age;
		};
		struct SIMUL_CLOUDS_EXPORT LightningProperties
		{
			Strike			strike;
			StrikeState		strikeState;
			int				seed;
			float			branch_angle_radians;
			float			branch_cosine,branch_sine;
			float			strikeThicknessMetres;
			float			roughness;
			float			motion;
			int				numLevels;
			int				numBranches;
			int				mask;
			int				mask_bits;
			vec3			colour;
			float			maxRadiance;
			float			minPixelWidth;
			int strikeIndex;
			const Storm *storm;
		};
	}
}


#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#endif
