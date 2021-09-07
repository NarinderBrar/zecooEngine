#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "ConeMesh.h"
#include "Transform.h";

class Cone:public Model
{
private:
	ConeMesh* coneMesh;

public:
	Cone(Material* material, Texture* texture);

	void render();

	~Cone();
};

