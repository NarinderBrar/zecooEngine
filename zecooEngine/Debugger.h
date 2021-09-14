#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Line.h";
#include "Ray.h";
#include "Cube.h";

#include "Camera.h";

#include <iostream>
#include <vector>

using namespace std;

class Debugger
{
public:
	Shader* shader;
	Material* material;

	Line* line;
	Ray* ray;
	Cube* cube;

	Camera* camera;

	vector<Line*> lines;
	vector<Ray*> rays;
	vector<Cube*> models;

	Debugger();
	Debugger(Camera* _camera);

	void printMsg(string txt);
	void printWarning(string txt);
	void printError(string txt);

	void addLine(glm::vec3 pointA, glm::vec3 pointB, glm::vec4 color);
	void addRay(glm::vec3 origin, glm::vec3 direction, glm::vec4 color);

	void addPoint(glm::vec3 position, glm::vec4 color);

	void draw();
};

