#include "Light.h"

using namespace simul;

opengl::Light::Light()
{
    mLightIndex = 0;
}

opengl::Light::~Light()
{
}

void opengl::Light::UpdateLight(const double * /*lLightGlobalPosition*/,float lConeAngle,const float lLightColor[4]) const
{
}