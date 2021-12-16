//  Copyright (c) 2018 Simul Software Ltd. All rights reserved.

#include "water_constants.sl"
#include "common.sl"
#include "render_states.sl"
#include "sampler_states.sl"
#include "simul_inscatter_fns.sl"
#include "debug_constants.sl"
#include "depth.sl"
#include "noise.sl"

#ifndef WATER_SL
#define WATER_SL
#ifndef PI
#define PI 3.1415926536f
#endif

#ifndef TAU
#define TAU 6.28318530718f
#endif

#define COS_PI_4_16 0.70710678118654752440084436210485f

#define PATCH_BLEND_BEGIN		200
#define PATCH_BLEND_END			1000

#ifndef MAX_FADE_DISTANCE_METRES
#define MAX_FADE_DISTANCE_METRES (300000.0)
#endif

//Random offset values for profile buffers to remove tiling and aliasing
/*
float profileOffsets[16] =
{
	0.54f,
	0.18f,
	0.61f,
	0.28f,
	0.09f,
	0.71f,
	0.99f,
	0.43f,
	0.11f,
	0.85f,
	0.24f,
	0.67f,
	0.31f,
	0.86f,
	0.13f,
	0.01f
};
*/
/*
vec2 profile16Directons[16] = 
{
	vec2(1.0f,   0.0f),
	vec2(0.92f,  0.38f),
	vec2(0.707f, 0.707f),
	vec2(0.38f,  0.92f),

	vec2(0.0f,   1.0f),
	vec2(-0.38f, 0.92f),
	vec2(-0.707f,0.707f),
	vec2(-0.92f, 0.38f),

	vec2(-1.0f,  0.0f),
	vec2(-0.92f,-0.38f),
	vec2(-0.707f,-0.707f),
	vec2(-0.38f,-0.92f),

	vec2(0.0f,  -1.0f),
	vec2(0.38f, -0.92f),
	vec2(0.707f,-0.707f),
	vec2(0.92f, -0.38f)
};
*/
/*
vec2 profile32Directons[32] =
{
	vec2(1.0f,    0.0f),
	vec2(0.98f,   0.19f),
	vec2(0.92f,   0.38f),
	vec2(0.83f,   0.55f),
	vec2(0.707f,  0.707f),
	vec2(0.55f,   0.83f),
	vec2(0.38f,   0.92f),
	vec2(0.19f,   0.98f),

	vec2(0.0f,    1.0f),
	vec2(-0.19f,  0.98f),
	vec2(-0.38f,  0.92f),
	vec2(-0.55f,  0.83f),
	vec2(-0.707f, 0.707f),
	vec2(-0.83f,  0.55f),
	vec2(-0.92f,  0.38f),
	vec2(-0.98f,  0.19f),

	vec2(-1.0f,  -0.0f),
	vec2(-0.98f, -0.19f),
	vec2(-0.92f, -0.38f),
	vec2(-0.83f, -0.55f),
	vec2(-0.707f,-0.707f),
	vec2(-0.55f, -0.83f),
	vec2(-0.38f, -0.92f),
	vec2(-0.19f, -0.98f),

	vec2(0.0f,   -1.0f),
	vec2(0.19f,  -0.98f),
	vec2(0.38f,  -0.92f),
	vec2(0.55f,  -0.83f),
	vec2(0.707f, -0.707f),
	vec2(0.83f,  -0.55f),
	vec2(0.92f,  -0.38f),
	vec2(0.98f,  -0.19f),
};
*/
struct TwoColourCompositeOutput
{
	vec4 add		SIMUL_RENDERTARGET_OUTPUT(0);
	vec4 multiply	SIMUL_RENDERTARGET_OUTPUT(1);
};

struct VS_OUTPUT
{
	vec4 Position : SV_POSITION;
	vec2 texCoords : TEXCOORD0;
	vec3 LocalPos : TEXCOORD1;
	vec4 vecColour : COLOR0;
	vec4 SurePosition : TEXCOORD3;
};

struct PS_WATER_OUTPUTS
{
	vec4 Depth			SIMUL_RENDERTARGET_OUTPUT(0);
	vec4 Scattering		SIMUL_RENDERTARGET_OUTPUT(1);
	vec4 Absorption		SIMUL_RENDERTARGET_OUTPUT(2);
	vec4 Normals		SIMUL_RENDERTARGET_OUTPUT(3);
	vec4 RefractColour	SIMUL_RENDERTARGET_OUTPUT(4);
};

vec3 convertPosToTexc(vec3 pos, uint3 dims)
{
	return (vec3(pos) + vec3(0.5, 0.5, 0.5)) / vec3(dims);
}

// Generating gaussian random number with mean 0 and standard deviation 1.
float Gauss(vec2 values)
{
	float u1 = rand(values.xy);
	float u2 = rand(values.yx * 0.5);
	if (u1 < 0.0000001f)
		u1 = 0.0000001f;
	return sqrt(-2.f* log(u1)) * cos(2.f*3.1415926536f * u2);
}

float spectrum(float zeta, float windSpeed) {
	float A = pow(1.1, 1.5 * zeta); // original pow(2, 1.5*zeta)
	float B = exp(-1.8038897788076411 * pow(4.f, zeta) / pow(windSpeed, 4.f));
	return 0.139098f * sqrt(A * B);
}

vec4 gerstner_wave(float phase , float knum) {
	float s = sin(phase);
	float c = cos(phase);

	return vec4(-s, c, -knum * c, -knum * s);
}

float cubic_bump(float x) {
	if (abs(x) >= 1)
		return 0.0f;
	else
		return x * x * (2 * abs(x) - 3) + 1;
}

//Calculate fresnel term
float fresnel(vec3 incident, vec3 normal, float sourceIndex, float mediumIndex)
{
	//float output; reserved wod, do not use
	//Schlick's approximation, 
	float cos_incident = clamp(-1.0, 1.0, dot(incident, normal));
	float R0 = pow(((mediumIndex - sourceIndex) / (mediumIndex + sourceIndex)), 2.0);
	return R0 + (1 - R0) * pow(1 - cos_incident, 5.0);

	/* Old more accurate but more expensive method
	float sint = (sourceIndex / mediumIndex) * sqrt(max(0.0, 1 - (cos_incident * cos_incident)));
	if (sint >= 1.0)
	{  
		output = 0.0;
	}
	else
	{
		float cost = sqrt(max(0.0, 1 - (sint * sint)));
		cos_incident = abs(cos_incident);
		float Rs = ((mediumIndex * cos_incident) - (sourceIndex * cost)) / ((mediumIndex * cos_incident) + (sourceIndex * cost));
		float Rp = ((sourceIndex * cos_incident) - (mediumIndex * cost)) / ((sourceIndex * cos_incident) + (mediumIndex * cost));
		output = ((Rs * Rs + Rp * Rp) / 2.0);
	}

	//float R = pow((sourceIndex - mediumIndex) / (sourceIndex + mediumIndex), 2);
	//output = R + (1 - R) * pow(1 - dot(incident, normal), 5);

	return output;
	*/
}

#endif