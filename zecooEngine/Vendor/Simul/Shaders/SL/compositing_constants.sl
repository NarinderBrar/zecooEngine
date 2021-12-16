//  Copyright (c) 2020 Simul Software Ltd. All rights reserved.
#ifndef COMP_CONSTANTS_SL
#define COMP_CONSTANTS_SL

SIMUL_CONSTANT_BUFFER(CompositingConstants,12)
	uniform mat4 worldToScatteringVolumeMatrix;
	uniform mat4 invViewProj;
	uniform vec4 viewportToTexRegionScaleBias;

	uniform vec2 offset;
	uniform float alpha;
	uniform float nearDist;					// near threshold for discarding depths.
	
	uniform vec4 colour2;
	uniform vec4 tanHalfFov;				// tanHalf and asymmetric offset

	uniform vec2 tanHalfFovUnused;
	uniform float exposure;
	uniform float gamma;

	uniform vec4 depthToLinFadeDistParams;
	uniform vec4 warpHmdWarpParam;

	uniform vec3 cubemapGroundColour;
	uniform float ccgcpad;

	uniform vec2 warpScreenCentre;
	uniform vec2 warpScale;

	uniform vec2 warpScaleIn;
	uniform vec2 padHdrConstants1;

	uniform uint2 hiResDimsX;
	uniform uint2 lowResDims;

	uniform uint2 fullResDims;
	uniform int numSamples;
	uniform float maxFadeDistanceKm;

	uniform vec3 infraredIntegrationFactors;
	uniform int randomSeed;

	uniform vec3 viewPos;
	uniform float fogExtinction;

	uniform vec3 fogColour;
	uniform float fogCeilingKm;

	uniform vec3 fogAmbient;
	uniform float anotherFloatXXX;
SIMUL_CONSTANT_BUFFER_END
	
#endif
