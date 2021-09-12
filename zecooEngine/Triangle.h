#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "TriangleMesh.h"
#include "TriangleIndexMesh.h"
#include "Transform.h";

class Triangle:public Model
{
private:
	TriangleIndexMesh* triangleIndexMesh;

public:
	Triangle(Material* material, Texture* texture);

	void render();

	~Triangle();
};

