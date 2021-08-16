#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "TriangleMesh.h"
#include "Transform.h";

class Triangle:public Model
{
private:
	TriangleMesh* triangleMesh;

public:
	Triangle(Material* material, Texture* texture);

	void render();

	~Triangle();
};

