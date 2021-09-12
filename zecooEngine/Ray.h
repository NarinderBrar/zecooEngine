#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "LineMesh.h";
#include "Transform.h";

class Ray :public Model
{
private:
	LineMesh* lineMesh;

public:
	Ray(Material* material);

	void set(glm::vec3 _origin, glm::vec3 _direction);
	void render();

	~Ray();
};