//  Copyright (c) 2015 Simul Software Ltd. All rights reserved.
#ifndef RAIN_CONSTANTS_SL
#define RAIN_CONSTANTS_SL

SIMUL_CONSTANT_BUFFER(RainConstants,10)
	uniform mat4 rainMapMatrix;
	uniform mat4 cubemapTransform;
	uniform vec4 lightColour;
	uniform vec3 lightDir;
	uniform float particleWidth;
	uniform vec3 meanFallVelocity;
	uniform float intensity;
	uniform float flurry;
	uniform float flurryRate;
	uniform float phase0;
	uniform float phase1;
	uniform float timeStepSeconds;
	uniform float particleZoneSize;
	uniform vec2 padding;
SIMUL_CONSTANT_BUFFER_END

SIMUL_CONSTANT_BUFFER(MoisturePerViewConstants,9)
	uniform vec4 depthViewport;		// xy = pos, zw = size
	uniform vec4 depthToLinFadeDist;
SIMUL_CONSTANT_BUFFER_END

SIMUL_CONSTANT_BUFFER(RainPerViewConstants, 8)
	mat4 worldViewProj[2];
	mat4 worldView[2];
	mat4 rainDepthTransform;
	uniform vec4 tanHalfFov;
	vec4 viewPos[2];
	vec4 offset[2];
	vec4 depthToLinFadeDistRain;
	vec4 depthToLinFadeDistTexture;
	vec4 viewportToTexRegionScaleBias;
	vec2 tanHalfFovUnused;
	vec2 screenSize;
	uniform vec3 viewPositionOffset;
	float nearRainDistance;// as a proportion of max fade distance
	float rainDepthTextureScale;
	float splashDelta;
	float minDistanceM;
	float padding2;
SIMUL_CONSTANT_BUFFER_END

struct PrecipitationVertex
{
    vec3 position;	
	uint type;
	vec3 velocity;
};
struct SplashVertex
{
    vec3 position;
	vec3 normal;
	float strength;
};

struct TransformedParticle
{
    vec4 position;
	vec3 view;
	float fade;
};

#endif