#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "Transform.h";

#include "TubeMesh.h";

class Tube :public Model
{
private:
	TubeMesh* tubeMesh;

public:
	Tube(Material* material, Texture* texture);

	void render();

	~Tube();
};

