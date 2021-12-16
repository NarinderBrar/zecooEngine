//  Copyright (c) 2015 Simul Software Ltd. All rights reserved.
#ifndef CLOUD_CONSTANTS_SL
#define CLOUD_CONSTANTS_SL

#define CLOUD_FADEIN_DIST 0.24

struct LayerData
{
	vec2 noiseOffset;
	float pad11;
	float pad12;
	float layerFade;
	float layerDistanceKm;
	float verticalShift;
	float pad13;
};

SIMUL_CONSTANT_BUFFER(CloudStaticConstants,12)
	uniform mat4 invViewProj[6];	// Cubemap matrices
SIMUL_CONSTANT_BUFFER_END


SIMUL_CONSTANT_BUFFER(CloudPerViewConstants,13)

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

SIMUL_CONSTANT_BUFFER(CloudConstants,9)
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

	uniform vec2 XXXXXXXXXXXXX;
	uniform vec2 rainTangent;

	uniform vec3 mieRayleighRatio;
	uniform float alphaSharpness;

	uniform float rainToSnow;
	uniform float maxFadeDistanceKm;
	uniform float extinctionPerKm;
	uniform float minSunlightAltitudeKm;

	uniform vec3 crossSectionOffset;
	uniform uint precipitationLayerCount;

	uniform vec3 noise3DTexcoordScale;
	uniform float rainEffect;

	uniform vec3 cloudIrRadiance1;
	uniform float yz;

	uniform vec3 cloudIrRadiance2;
	uniform float MaxNoiseAmplitudeKm;

	uniform vec3 directionToMoon;
	uniform float baseNoiseFactorDeprecated;

	uniform vec3 noise3DTexcoordOffset;
	uniform float dropletRadius;
	
	uniform vec3 worleyTexcoordScale;
	uniform float precipitationThreshold;

	uniform vec3 rainCentreKm;
	uniform float rainRadiusKm;

	uniform vec3 rainNoiseInvScale;
	uniform float rainVerticalTexcoord;

	uniform vec3 worleyOffsetKm;
	uniform float precipitation;

	uniform vec3 noise3DTexcoordOffset2;
	uniform float rainbowDepthPoint;

	uniform vec3 noise3DTexcoordScale2;
	uniform uint allowOccludedRainbows;

	uniform uint3 precipitationGrid;
	uniform uint allowLunarRainbows;
	
	uniform vec3 angleRadiansAltitudeKmRange;
	uniform uint cloudRandomSeed;

	uniform float rainEdgeKm;
	uniform float cc_pad1;
	uniform vec2 cc_pad2;
	uniform vec3 windOffsetKm;
	uniform float wo_pad1;
SIMUL_CONSTANT_BUFFER_END

SIMUL_CONSTANT_BUFFER(CloudLightpassConstants,10)
	uniform vec3 sourcePosKm;
	uniform float sourceRadiusKm;
	uniform vec3 spectralFluxOver1e6;			// Units of watts per nm
	uniform float minCosine;
	uniform float irradianceThreshold;
	uniform float maxRadiusKm;
	uniform vec2 padding2;
SIMUL_CONSTANT_BUFFER_END

//! The result struct for a point or volume query.
struct VolumeQueryResult
{
	vec4 pos_km;
	int valid;
	float density;
	float direct_light;
	float indirect_light;
	float ambient_light;
	float precipitation;
	float rain_to_snow;
	float padding;
};
//! The result struct for a line query.
struct LineQueryResult
{
	vec4 pos1_km;
	vec4 pos2_km;
	vec3 padding2;
	int valid;
	float density;
	float visibility;
	float optical_thickness_km;
	float first_contact_km;
};

//! The information struct for a single layer of cloud precipitation.
struct PrecipitationLayerInfo
{
	float precipitation;
	float rainToSnow;
	float cloudBaseKm;
	float cloudLayerThicknessKm;
	float precipitationBaseKm;
	float rainRadiusKm;
	vec2 rainCentreKm;
	vec3 mapScale;
	uint cloudMaskTextureIndex;
	vec3 mapOffset;
	float azimuth;
};
#ifdef __cplusplus
namespace simul
{
	namespace crossplatform
	{
		class Texture;
		class SamplerState;
	}
}

//! A struct containing a pointer for the cloud shadow texture, along with 
//! information on how to project it.
struct CloudShadowStruct 
{
	/// The cloud texture.
	simul::crossplatform::Texture *cloudTexture;
	/// The cloud light texture.
	simul::crossplatform::Texture *lightTexture;
	/// The 2D cloud shadow texture.
	simul::crossplatform::Texture *cloudShadowTexture;	
	/// The rain map texture.
	simul::crossplatform::Texture * precipitation_volume;			/// Texture represents where in the horizontal plane of the cloud rain can fall.
	/// Transform a position from shadow space to world space
	mat4 shadowMatrix;
	mat4 simpleOffsetMatrix;
	/// Matrix to transform a position from world space to cloud or shadow space
	mat4 worldspaceToCloudspaceMatrix;
	float minAltKm;
	float maxAltKm;
	float shadowRangeKm;
	float averageCoverage;				///< Between 0 and 1, blend shadows to this in the distance.
	float rainbowIntensity;
	float godrays_strength;
	bool wrap;
	simul::crossplatform::SamplerState *samplerState;
	vec3 originKm;
	vec3 scaleKm;
	unsigned checksum = 0;
};
#else
#endif
#endif