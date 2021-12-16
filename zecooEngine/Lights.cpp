#include "Lights.h"

Lights::Lights()
{

}

Lights::~Lights()
{

}

DirectionalLights::DirectionalLights(glm::vec3 _position, glm::vec3 _direction)
{
	position = _position;
	direction = _direction;
}

DirectionalLights::~DirectionalLights()
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