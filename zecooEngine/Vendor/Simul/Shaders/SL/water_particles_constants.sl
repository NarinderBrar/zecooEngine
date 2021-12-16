//  Copyright (c) 2020 Simul Software Ltd. All rights reserved.
#ifndef WATER_PARTICLES_CONSTANTS_SL
#define WATER_PARTICLES_CONSTANTS_SL

// Per draw call constants
SIMUL_CONSTANT_BUFFER(cbParticleRenderConstants,1)
	// Transform matrices
	uniform mat4		g_matWorldViewProj;
	uniform mat4		g_matInvViewProj;
	uniform mat4		g_matWorld;

	// Misc per draw call constants
	uniform vec2		g_UVBase;
	uniform vec2		g_objectCenter;

	uniform vec3		g_LocalEye;
	uniform float		hgfdsgfdce;
	 
	SIMUL_CONSTANT_BUFFER_END

//Particle Generation constants
SIMUL_CONSTANT_BUFFER(cbParticleGenerationConstants, 2)
	uniform mat4		g_planeMat;

	uniform int			g_offset;
	uniform float		g_maxLifetime;
	uniform float		g_minLifetime;
	uniform float		g_averageMass;

	uniform vec3		g_velocity;
	uniform int			g_maxParticles;

	uniform vec3		g_position;
	uniform int			g_maxTotalParticles;

	uniform vec2		g_planeScale;
	uniform vec2		fduioshfiu;

	uniform vec3		g_planeVelocity;
	uniform float		fndjsonf;

SIMUL_CONSTANT_BUFFER_END

//Particle Update constants
SIMUL_CONSTANT_BUFFER(cbParticleUpdateConstants, 3)
	uniform vec3		gnfjxoin;
	uniform float		g_dt;

SIMUL_CONSTANT_BUFFER_END

//Particle Generation Plane constants
SIMUL_CONSTANT_BUFFER(cbParticleGenerationPlaneConstants, 4)
uniform vec2		g_surfaceUVScale;
uniform vec2		g_surfaceUVOffset;

uniform vec3		g_surfaceCenter;
uniform float		g_surfaceProfileUVScale;

uniform float		g_surfaceWindDirection;
uniform float		g_surfaceWindDependancy;
uniform float		g_surfaceWaveAmplitude;
uniform bool		g_enableWaveGrid;

uniform vec3		g_scale;
uniform int			jfgidsjoi;

uniform mat4		g_generationPlaneMat;
uniform mat4		g_rotationPlaneMat;

SIMUL_CONSTANT_BUFFER_END


struct particleData
{
	vec3 position;
	vec3 velocity;
	float age;
	float mass;
	vec4 colour;
};

#endif