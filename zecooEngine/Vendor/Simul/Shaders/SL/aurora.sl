//  Copyright (c) 2020 Simul Software Ltd. All rights reserved.
#ifndef AURORA_SL
#define AURORA_SL

#include "simul_refract_reflect.sl"

//permittivity of free space s4·A2/(m3·kg)
#define SIMUL_E0_F 8.8541878128e-12
//charge on an electron (C)
#define SIMUL_QE_F -1.602176634e-19
//rest mass of an electron (kg)
#define SIMUL_ME_F 9.10938370156e-31
//gravitational acceleration m/s2
#define SIMUL_EARTH_G 9.80665 
//molar mass of Earth's air kg/mol
#define SIMUL_EARTH_AIR_MOLAR_MASS 0.0289644 
//universal gas constant  N·m/(mol·K)
#define SIMUL_R_F 8.3144598 

//Density equation in kg/m3 : 
//<100Km:		https://en.wikipedia.org/wiki/Barometric_formula Equation 2
//100Km-500Km:	https://www.spaceacademy.net.au/watch/debris/atmosmod.htm
float AtmosphericDensity(float altitude_metres)
{
	if (altitude_metres / 1000 <= 180)
	{
		float h = altitude_metres / 1000;
		float a0 = +7.001985e-2;
		float a1 = -4.336216e-3;
		float a2 = -5.009831e-3;
		float a3 = +1.621827e-4;
		float a4 = -2.471283e-6;
		float a5 = +1.904383e-8;
		float a6 = -7.189421e-11;
		float a7 = +1.060067e-13;
		float polyfn = ((((((a7 * h + a6) * h + a5) * h + a4) * h + a3) * h + a2) * h + a1) * h + a0;
		return pow(10, polyfn);
	}
	else if (altitude_metres / 1000 <= 500)
	{
		float F10 = (300.0 + 70.0) / 2.0;
		float Ap = 0.0;
		float T = 900.0 + 2.5 * (F10 - 70.0) + 1.5 * Ap;
		float u = 27.0 - 0.012 * (altitude_metres / 1000 - 200.0);
		float H = T / u;
		return 6.0e-10 * exp(-(altitude_metres / 1000 - 175.0) / H);
	}
	else // Above 500km, we assume the atmosphereic density is negligible and thus zero.
		return 0.0;
}

//Sum of Densities from altitude_metres to 500km in kg/m2
float AtmosphericTotalShieldingMass(float altitude_metres)
{
	float h = 50;
	float range = 500 * 1000 - altitude_metres;
	float dx = range / h;
	float result = 0;

	for (int i = 0; i < h; i++)
	{
		result += AtmosphericDensity(altitude_metres + i * dx) * dx;
	}
	return result;
}

//initialEnergy in keV, 0keV < 32keV; altitude in metres
float AuroralEnergyDepositionRate(float initialEnergy, float altitude_metres)
{
	float E = initialEnergy;
	float z = altitude_metres;
	float Dz = AtmosphericDensity(z) / 1000.0; //in g/cm3. This is listed directly in the MSIS data;
	float Mz = AtmosphericTotalShieldingMass(z) / 10.0; //g/cm2.
	float Me = 4.6e-6 * pow(E, 1.65);
	float r = (Mz / Me);
	float L = 4.2 * r * exp(-(r * r) - r) + 0.48 * exp(-17.4 * pow(r, 1.37));
	float Az = L * E * (Dz / Me);
	return Az * 1000000;
}

//Equation from https://brilliant.org/wiki/ohms-law-microscopic-interpretation
//Rough values from https://www.scielo.br/scielo.php?script=sci_arttext&pid=S1806-11172014000400012
float Conductivity(float electronVolumeDensity, float electronFreeTime)
{
	float sigma;
	sigma = electronVolumeDensity * SIMUL_QE_F * SIMUL_QE_F * (electronFreeTime / SIMUL_ME_F);
	return sigma;
}

//---Helper Functions---
vec3 DirectionToColour(vec2 p)
{
	float theta = -atan2(p.y, p.x);
	if (theta < 0)
		theta += SIMUL_TAU_F;

	float r = 0;
	float g = 0;
	float b = 0;

	if (theta > SIMUL_TAU_F * 0.0 / 3.0 && theta < SIMUL_TAU_F * 1.0 / 3.0)
	{
		r = 1.0 - NormalizeLERPPointWithinRange(SIMUL_TAU_F * 0.0 / 3.0, SIMUL_TAU_F * 1.0 / 3.0, theta);
		g = 1.0 - r;
		b = 0;
	}
	if (theta > SIMUL_TAU_F * 1.0 / 3.0 && theta < SIMUL_TAU_F * 2.0 / 3.0)
	{
		g = 1.0 - NormalizeLERPPointWithinRange(SIMUL_TAU_F * 1.0 / 3.0, SIMUL_TAU_F * 2.0 / 3.0, theta);
		b = 1.0 - g;
		r = 0;
	}
	if (theta > SIMUL_TAU_F * 2.0 / 3.0 && theta < SIMUL_TAU_F * 3.0 / 3.0)
	{
		b = 1.0 - NormalizeLERPPointWithinRange(SIMUL_TAU_F * 2.0 / 3.0, SIMUL_TAU_F * 3.0 / 3.0, theta);
		r = 1.0 - b;
		g = 0;
	}

	return vec3(r, g, b) * length(p);
}

//Latitude must be greater than + or - 60 degrees.
vec2 LatLongToCentralisedTextureCoordinates(vec2 latLong)
{
	float normalisedRadius = NormalizeLERPPointWithinRange(90.0, 60.0, latLong.x);
	float theta = latLong.y - 90.0;
	if (theta < 0.0)
		theta += 360.0;

	float x = 0.5 * normalisedRadius * cos(theta * SIMUL_PI_F / 180.0);
	float y = 0.5 * normalisedRadius * sin(theta * SIMUL_PI_F / 180.0);
	
	return vec2(x, y);
}
//Latitude must be greater than + or - 60 degrees.
vec2 LatLongToCornerTextureCoordinates(vec2 latLong)
{
	return LatLongToCentralisedTextureCoordinates(latLong) + vec2(0.5, 0.5);
}

#endif