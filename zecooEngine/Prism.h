#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "PrismIndexMesh.h"
#include "Transform.h";

class Prism :public Model
{
private:
	PrismIndexMesh* prismIndexMesh;

public:
	Prism(Material* material, Texture* texture);

	void render();

	~Prism();
};

