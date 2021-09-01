#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "PyramidMesh.h"
#include "Transform.h";

class Pyramid :public Model
{
private:
	//TriangleMesh* triangleMesh;
	PyramidMesh* pyramidMesh;

public:
	Pyramid(Material* material, Texture* texture);

	void render();

	~Pyramid();
};

