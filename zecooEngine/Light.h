#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include "Shader.h"

class Light
{
private:

public:
	glm::vec3 position = glm::vec3(0,0,0);
	glm::vec3 direction = glm::vec3(0.5f, -1.0f, 0.5f);

	glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);

	float constant = 1.0f;
	float lineartant = 0.09f;
	float quadratic = 0.032f;

    Light();
	~Light();
};

class DirectionalLight:public Light
{
public:
	DirectionalLight(glm::vec3 _position, glm::vec3 _direction);
	~DirectionalLight();

private:

};

class PointLight:public Light
{
public:
	PointLight(glm::vec3 _position, glm::vec3 _direction);
	~PointLight();

private:

};

class SpotLight :public Light
{
public:
	float cutOff = glm::cos(glm::radians(12.5f));
	float outerCutOff = glm::cos(glm::radians(15.0f));

	SpotLight(glm::vec3 _position, glm::vec3 _direction);
	~SpotLight();

private:

};

