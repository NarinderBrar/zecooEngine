//  Copyright (c) 2015 Simul Software Ltd. All rights reserved.
#ifndef GPU_SKY_CONSTANTS_SL
#define GPU_SKY_CONSTANTS_SL

SIMUL_CONSTANT_BUFFER(GpuSkyConstants,8)
	uniform vec2 texSize;
	uniform vec2 tableSize;
	
	uniform uint3 threadOffset;
	uniform float emissivity;

	uniform vec3 zenith;
	uniform float distanceKm;

	uniform vec3 horizon;
	uniform float brightnessPower;
	uniform float texelOffset;
	uniform float prevDistanceKm;

	uniform float maxOutputAltKm;
	uniform float planetRadiusKm;

	uniform float maxDensityAltKm;
	uniform float hazeBaseHeightKm;
	uniform float hazeScaleHeightKm;
	uniform float seaLevelTemperatureK;

	uniform vec3 rayleigh;
	uniform float atmosphereThicknessKm;
	uniform vec3 hazeMie;
	uniform float XovercastRangeKmX;
	uniform vec3 ozone;
	uniform float sunRadiusRadians;		// as proportion of sunlight

	uniform vec3 sunIrradiance;
	uniform float maxDistanceKm;

	uniform vec3 lightDir;
	uniform float hazeEccentricity;

	uniform vec3 starlight;
	uniform float previousZCoord;

	uniform vec3 mieRayleighRatio;
	uniform float minOutputAltKm;

	uniform vec4 yRange;

	uniform float texCoordZ;
	uniform float blendToColours;
	uniform float interpColours;
	uniform float sun_start_alt_km;

	uniform uint3 targetSize;
	uniform int numMoons;

	uniform vec4 moonIrradianceFactor[10];		// as proportion of sunlight
	uniform vec4 directionToMoon[10];
SIMUL_CONSTANT_BUFFER_END
#endif