#include "Light.h"

Light::Light()
{

}

Light::~Light()
{

}

DirectionalLight::DirectionalLight(glm::vec3 _position, glm::vec3 _direction)
{
	position = _position;
	direction = _direction;
}

DirectionalLight::~DirectionalLight()
{
}

PointLight::PointLight(glm::vec3 _position, glm::vec3 _direction)
{
	position = _position;
	direction = _direction;
}

PointLight:: ~PointLight()
{
}

SpotLight::SpotLight(glm::vec3 _position, glm::vec3 _direction)
{
	position = _position;
	direction = _direction;
}

SpotLight::~SpotLight()
{
}