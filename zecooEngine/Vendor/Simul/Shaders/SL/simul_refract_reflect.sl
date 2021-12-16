//  Copyright (c) Simul Software Ltd. All rights reserved.
#include "../SL/common.sl"

//Probablity of Reflection
float ReflectionFresnelSchlick(float incidentAngle, float riOld, float riNew)
{
	// NEVER PASS A NEGATIVE OPERAND TO POW()!!!
	float a=abs(riNew-riOld)/(riNew+riOld);
	float R0 = a*a;
	float c=1.0-cos(incidentAngle);
	float c2=c*c;
	float oneTakeCos5 = c2*c2*c;
	return lerp(R0,1.0,oneTakeCos5);
}

//Angle of Refraction
float RefractionSnell(float incidentAngle, float riOld, float riNew)
{
	float riRatio				= riOld / riNew;
	float sinOfIncidentAngle	= sin(incidentAngle);
	float temp					= riRatio * sinOfIncidentAngle;
	if (temp > 1.0f)
	{
		return SIMUL_TAU_F - incidentAngle;
	}
	else
	{
		return asin(temp);
	}
}

//Return the Refeactive Index of water dependent on the wavelength of the visible light in nm (400nm - 700nm)
float WaterRefractiveIndex(float wavelength)
{
	return 1.339 * exp((400.0 - wavelength) / 50000);
}

vec3 WavelengthToRGB(float wavelength)
{
	float normalisedWavelength = (wavelength - 400.0) / 300.0;
	
	float r = cos(2.0 * SIMUL_PI_F * (normalisedWavelength - (625.0 - 400.0) / 300.0));
	float g = cos(2.0 * SIMUL_PI_F * (normalisedWavelength - (555.0 - 400.0) / 300.0));
	float b = cos(2.0 * SIMUL_PI_F * (normalisedWavelength - (475.0 - 400.0) / 300.0));

	if (normalisedWavelength > 0.5)
		b = 0.0;
	else
		r = 0.05 * cos(12.00 * (normalisedWavelength - (440.0 - 400.0) / 300.0));

	//if (normalisedWavelength > ((555.0 - 400.0) / 300.0) && normalisedWavelength < 0.95)
	//{
	//	g = cos(SIMUL_PI_F * SIMUL_PI_F * (normalisedWavelength - (555.0 - 400.0) / 300.0));
	//}
	if (normalisedWavelength.x < ((470.0 - 400.0) / 300.0))
	{
		b = cos(2.15 * SIMUL_PI_F * (normalisedWavelength - (470.0 - 400.0) / 300.0));
	}

	return vec3(r, g, b);
}

vec3 WavelengthToRGB_Weighted(float wavelength, vec3 rgb)
{
	vec3 unweighted = WavelengthToRGB(wavelength);
	return vec3(unweighted.r * rgb.r, unweighted.g * rgb.g, unweighted.b * rgb.b);
}

float GetRefractedAngle(float ri)
{
	return acos(sqrt((4.0 / 3.0) - (4.0 / (3.0 * pow(ri, 2)))));
}

float NormalizeLERPPointWithinRange(float x0, float x1, float t)
{
	return (t - x0) / (x1 - x0);
}

vec3 lerp_vec3(vec3 v0, vec3 v1, float t) 
{
	vec3 result;
	result.x = (1 - t) * v0.x + t * v1.x;
	result.y = (1 - t) * v0.y + t * v1.y;
	result.z = (1 - t) * v0.z + t * v1.z;

	return result;
}

vec3 SpectualEffectsDueToDropletSize(float dropletDiameter)
{
	vec3 _1000um = vec3(1.00, 1.00, 1.00) * (3.00/3.00);
	vec3 _0500um = vec3(0.50, 0.75, 1.00) * (3.00/2.25);
	vec3 _0250um = vec3(0.25, 0.50, 1.00) * (3.00/1.75);
	vec3 _0100um = vec3(0.25, 0.25, 1.00) * (3.00/1.50);
	vec3 _0050um = vec3(0.00, 0.00, 0.00) * (3.00/3.00);

	if (dropletDiameter >= 1.0)
		return _1000um;
	else if (dropletDiameter >= 0.50 && dropletDiameter < 1.00)
		return lerp_vec3(_0500um, _1000um, NormalizeLERPPointWithinRange(0.50, 1.00, dropletDiameter));
	else if (dropletDiameter >= 0.25 && dropletDiameter < 0.50)
		return lerp_vec3(_0250um, _0500um, NormalizeLERPPointWithinRange(0.25, 0.50, dropletDiameter));
	else if (dropletDiameter >= 0.10 && dropletDiameter < 0.25)
		return lerp_vec3(_0100um, _0250um, NormalizeLERPPointWithinRange(0.10, 0.25, dropletDiameter));
	else if (dropletDiameter >= 0.05 && dropletDiameter < 0.10)
		return lerp_vec3(_0050um, _0100um, NormalizeLERPPointWithinRange(0.05, 0.10, dropletDiameter));
	else
		return vec3(0, 0, 0);
}

