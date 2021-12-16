//  Copyright (c) 2020 Simul Software Ltd. All rights reserved.
#ifndef AURORA_CONSTANTS_SL
#define AURORA_CONSTANTS_SL

SIMUL_CONSTANT_BUFFER(AuroraConstants, 5)

uniform vec2  FAC_Region1_ArcDawn_centrePoint;
uniform float FAC_Region1_ArcDawn_startAzimuth;
uniform float FAC_Region1_ArcDawn_endAzimuth;
uniform float FAC_Region1_ArcDawn_radius;
uniform float FAC_Region1_ArcDawn_strength;

uniform vec2  FAC_Region1_ArcDusk_centrePoint;
uniform float FAC_Region1_ArcDusk_startAzimuth;
uniform float FAC_Region1_ArcDusk_endAzimuth;
uniform float FAC_Region1_ArcDusk_radius;
uniform float FAC_Region1_ArcDusk_strength;

uniform vec2  FAC_Region2_ArcDawn_centrePoint;
uniform float FAC_Region2_ArcDawn_startAzimuth;
uniform float FAC_Region2_ArcDawn_endAzimuth;
uniform float FAC_Region2_ArcDawn_radius;
uniform float FAC_Region2_ArcDawn_strength;

uniform vec2  FAC_Region2_ArcDusk_centrePoint;
uniform float FAC_Region2_ArcDusk_startAzimuth;
uniform float FAC_Region2_ArcDusk_endAzimuth;
uniform float FAC_Region2_ArcDusk_radius;
uniform float FAC_Region2_ArcDusk_strength;

uniform vec3 globalDirectionSun;
uniform uint auroraLayersCount;

uniform float AGC_highestLatitude;
uniform float AGC_lowestLatitide;
uniform float AGC_maxAuroralBand;
uniform float AGC_minAuroralBand;

uniform float auroralLayerIntensity;
uniform float time;
uniform float atmosphericElectronVolumeDensity;
uniform float atmosphericElectronFreeTime;

uniform vec2 acPad;
uniform uint intensityMapDrawFrameNumber;
uniform uint auroraTraceLength;

SIMUL_CONSTANT_BUFFER_END

SIMUL_CONSTANT_BUFFER(AuroraPerViewConstants, 6)

uniform mat4 worldViewProj;
uniform mat4 worldViewProj_Inv;

uniform vec2 cloudWindowLatLonPos;
uniform vec2 cloudWindowSize;

uniform float cloudWindowHeading;
uniform float cloudWindowHeight;
uniform float planetRadius;
uniform float apvcPad;

SIMUL_CONSTANT_BUFFER_END

struct AuroralLayer
{
	float base;					//! Base of the Auroral Layer in km.
	float top;					//! Top of the Auroral Layer in km.
	float emittedWavelength;	//! Wavelength of the emitted light in nm.
	float strength;				//! Strength of the Emitted Light in kR (KiloRayleigh).
};

#endif