#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Line.h";
#include "Ray.h";
#include "Camera.h";

#include <iostream>
using namespace std;

class Debugger
{
public:
	Shader* shader;
	Material* material;

	Line* line;
	Ray* ray;
	Camera* camera;

	void printMsg(string txt);
	void printWarning(string txt);
	void printError(string txt);

	void createLine(glm::vec3 pointA, glm::vec3 pointB, Camera* camera);
	void createRay(glm::vec3 origin, glm::vec3 direction, Camera* camera);

	void drawLine();
	void drawRay();
};

