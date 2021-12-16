//  Copyright (c) 2020 Simul Software Ltd. All rights reserved.
#ifndef WATER_PARTICLES_SL
#define WATER_PARTICLES_SL

#include "shader_platform.sl"

#include "common.sl"
#include "render_states.sl"
#include "sampler_states.sl"

#include "common.sl"

#ifndef TAU
#define TAU 6.28318530718f
#endif

struct VS_OUTPUT
{
	vec4 Position : SV_POSITION;
	vec2 texCoords : TEXCOORD0;
	vec3 LocalPos : TEXCOORD1;
	vec4 vecColour : COLOR0;
	vec4 SurePosition : TEXCOORD3;
};

float nrand(vec2 uv)
{
	return frac(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
}

uint wang_hash(uint value)
{
	uint seed = value;
	seed = (seed ^ 61) ^ (seed >> 16);
	seed *= 9;
	seed = seed ^ (seed >> 4);
	seed *= 0x27d4eb2d;
	seed = seed ^ (seed >> 15);
	return seed;
}

uint rand_xorshift(uint value)
{
	uint rng_state = value;

	// Xorshift algorithm from George Marsaglia's paper
	rng_state ^= (rng_state << 13);
	rng_state ^= (rng_state >> 17);
	rng_state ^= (rng_state << 5);
	return rng_state;
}

#endif