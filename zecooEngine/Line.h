#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "LineMesh.h";
#include "Transform.h";

class Line :public Model
{
private:
	LineMesh* lineMesh;

public:
	Line(Material* material);

	void setPoints(glm::vec3 _pointA, glm::vec3 _pointB);
	void render();

	~Line();
};

