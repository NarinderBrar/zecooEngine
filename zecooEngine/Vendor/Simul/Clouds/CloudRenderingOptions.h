#pragma once
#include "Simul/Base/Variant.h"
#include "Simul/Clouds/Export.h"
#include "Platform/CrossPlatform/Quaterniond.h"
#include "Platform/CrossPlatform/AmortizationStruct.h"

#ifdef _MSC_VER
	#pragma warning(push)  
	#pragma warning(disable : 4251)  
#endif
#ifdef UNIX
	#include "stdint.h"
#endif
namespace simul
{
	namespace clouds
	{
		enum RenderingMode
		{
			STANDARD_RENDERING=0		// Using the grid raytrace method
			,SIMPLIFIED_RENDERING=1		// A naive raytrace algorithm
			,VARIABLE_STEP_GRID=2		// A simple raytrace that keeps steps consistent.
			,SIMPLIFIED_CONSISTENT_RENDERING=3	// A simple raytrace that keeps steps consistent.
		};
		enum LightingMode
		{
			STANDARD_LIGHTING=0			// Using the lighting raytrace method with a loop in the shader
			,INCREMENTAL_LIGHTING=1		// Using the adjacent light values to update the texture.
			,GPU_RAYTRACE_LIGHTING=2	// Not yet implemented.
		};
		enum VolumeUpdateMode:uint8_t
		{
			NEVER=0						// Don't update the texture.
			,SMOOTH=1					// Blend to the new value.
			,FULL=2						// Completely update.
		};
		/*! This controls the rendering of clouds.
		*/
		struct SIMUL_CLOUDS_EXPORT CloudRenderingOptions
		{
		public:
			CloudRenderingOptions();

			float			HighDetailProportion;			//!< For cloud volume update rate.
			float			MediumDetailProportion;			//!< For medium cloud volume update rate.

			int				MaximumCubemapResolution;		//!< Resolution to draw full-detail cloud buffers

			float			CloudShadowStrength;
			float			CloudShadowRangeKm;
			int				ShadowTextureSize;

			float			CrepuscularRayStrength;
			uint3			GodraysGrid;
			int				MaxPrecipitationParticles;
			float			PrecipitationRadiusMetres;
			float			RainFallSpeedMS;
			float			RainDropSizeMm;
			float			SnowFallSpeedMS;
			float			SnowFlakeSizeMm;
			float			PrecipitationWindEffect;
			float			PrecipitationWaver;
			float			PrecipitationWaverTimescaleS;
			float			PrecipitationThresholdKm;

			bool			AutomaticRainbowPosition;
			float			RainbowElevation;
			float			RainbowAzimuth;
			float			RainbowIntensity;
			float			RainbowDepthPoint;
			bool			AllowOccludedRainbow;
			bool			AllowLunarRainbow;

			float			EdgeNoisePersistence;
			int				EdgeNoiseFrequency;
			int				EdgeNoiseTextureSize;
			float			EdgeNoiseWavelengthKm;
			int				WorleyTextureSize;
			float			CellNoiseWavelengthKm;
			float			MaxFractalAmplitudeKm;
			float			MaxCloudDistanceKm;
			float			RenderGridXKm;					//!< Minimum grid width for raytracing.
			float			RenderGridZKm;					//!< Minimum grid height for raytracing.
			RenderingMode	RaytraceMode;					//!< Hint for the renderer on how to raytrace these clouds.
			LightingMode	lightingMode;
			int				DefaultNumSlices;
			int				DefaultAmortization;
			vec3			WindSpeedMS;
			bool			RealTimeWind;
			float			CloudThresholdDistanceKm;
			float			CloudDepthTemporalAlpha;

			float			DirectLight;					//!< The amount of direct light to be used for rendering.
			float			IndirectLight;					//!< The amount of indirect or secondary light to be used for rendering.
			float			AmbientLight;					//!< The amount of ambient light to be used for rendering.
			float			Extinction;						//!< The amount of light scattered per metre - larger values produce darker clouds, default 0.05.
			float			MieAsymmetry;					//!< Mie scattering eccentricity.
			
			float			MinimumStarPixelSize;			//!< Smallest pixel width to use drawing stars.
			float			StarBrightness;					//!< Brightness multiplier for stars.
			float			CosmicBackgroundBrightness;		//!< Brightness multiplier for cosmic background.
			int				MaximumStarMagnitude;			//!< Largest magnitude of star to draw. Larger magnitudes are dimmer.

			vec3			CloudTint;

			vec2			WindowScaleKm;
			int2			WindowGrid;
			int2			PrecipitationGridDivisor;

			VolumeUpdateMode		volumeUpdateMode;
			static unsigned long long GetEnum(const char *txt_utf8);
			base::Variant GetValue(unsigned long long Enum) const;
			bool SetValue(unsigned long long Enum,base::Variant f);
		};
	}
}

#ifdef _MSC_VER
	#pragma warning(pop)  
#endif