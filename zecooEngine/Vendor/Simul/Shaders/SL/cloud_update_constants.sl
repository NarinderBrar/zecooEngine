//  Copyright (c) 2020 Simul Software Ltd. All rights reserved.
#ifndef CLOUD_UPDATE_CONSTANTS_SL
#define CLOUD_UPDATE_CONSTANTS_SL

struct LayerData2
{
	vec2 noiseOffset;
	float pad11;
	float pad12;
	float layerFade;
	float layerDistanceKm;
	float verticalShift;
	float pad13;
};
/*
SIMUL_CONSTANT_BUFFER(CloudUpdateConstants,8)
	uniform int3 updateIdxOffset;
	uniform int zPixel;
	uniform float diffusivity;
	uniform float edgeSharpness;
	uniform float baseNoiseFactor;
	uniform float localNoiseStrength;
	uniform float localCellNoiseStrength;
SIMUL_CONSTANT_BUFFER_END*/

SIMUL_CONSTANT_BUFFER(CloudPerViewConstants2,13)

	uniform uint4 targetRange[6];

	uniform mat4 shadowMatrix;		// Transform from texcoords xy to world viewplane XYZ

	uniform mat4 worldToScatteringVolumeMatrix;
	uniform mat4 volumeToLayerMatrix;
	
	uniform vec4 depthToLinFadeDistParams;

	uniform vec3 scaleOfGridCoordsKm;
	uniform int halfClipSize;			//  half the full clip size.

	uniform vec3 gridOriginPosKm;
	uniform int cubemapViewIndex;

	uniform vec3 viewPosKm;
	uniform float sampleHeight;

	uniform float shadowRangeKm;
	uniform int shadowTextureSize;
	uniform int raytraceSteps;
	uniform int cubemapTargetIndex;

	uniform int3 amortizationOffset;
	uniform float exposure;
	
	uniform uint2 targetTextureSize;
	uniform uint2 edge;

	uniform uint3 amortizationScale;
	uniform float maxCloudDistanceKm;

	uniform uint4 cubemapFaceIndex[6];

	uniform vec3 scale;
	uniform float cloud_interp;

	uniform vec3 offset;
	uniform float azimuth;
	
	uniform vec3 mapScale;
	uniform int initialSteps;

	uniform vec3 mapOffset;
	uniform uint stepPos;

	uniform int2 gridBackCornerTexel;
	uniform int2 gridCentreTexel;

	uniform int4 exclusionRange;
	
	uniform vec2 cirrusWindVector;
	uniform int thicknessSteps;
	uniform float initialVolumeMultiplier;

	uniform vec3 cloudTintColour;
	uniform float windowScaleKm_X;

	uniform uint3 windowGrid;
	uniform int firstInput;

SIMUL_CONSTANT_BUFFER_END

SIMUL_CONSTANT_BUFFER(CloudConstants2,9)
	uniform vec3 inverseScalesKm;
	uniform float rainbowIntensity;

	uniform vec3 VolumeScaleKm;
	uniform uint rainbowDirectionOverride; //0 = No Override, 1 = Override at day, 2 = Override at night

	uniform vec3 ambientColour;
	uniform int stepCycle;

	uniform vec3 fractalScale;
	uniform float cloudEccentricity;

	uniform vec4 lightResponse;

	uniform vec3 directionToSun;
	uniform float earthshadowMultiplier;

	uniform vec3 cornerPosKm;
	uniform float hazeEccentricity;

	uniform vec3 sunlightColour1;
	uniform int maxMip;

	uniform vec3 sunlightColour2;
	uniform float fadeAltitudeRangeKm;

	uniform vec3 mieRayleighRatio;
	uniform float alphaSharpness;

	uniform float rainToSnow;
	uniform float maxFadeDistanceKm;
	uniform float extinctionPerKm;
	uniform float minSunlightAltitudeKm;

	uniform vec3 crossSectionOffset;
	uniform uint precipitationLayerCount;

	uniform vec3 directionToMoon;
	uniform float rainEffect;

	uniform float dropletRadius;
	uniform float rainbowDepthPoint;
	uniform uint allowOccludedRainbows;
	uniform float rainVerticalTexcoord;
	
	uniform vec3 worleyTexcoordScale;
	uniform float precipitationThreshold;

	uniform vec3 rainCentreKm;
	uniform float rainRadiusKm;

	uniform vec3 worleyTexcoordOffset;
	uniform float precipitation;

	uniform uint3 precipitationGrid;
	uniform uint allowLunarRainbows;
	
	uniform vec3 angleRadiansAltitudeKmRange;
	uniform uint cloudRandomSeed;

	uniform float rainEdgeKm;
SIMUL_CONSTANT_BUFFER_END

#endif