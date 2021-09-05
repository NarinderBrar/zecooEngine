#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "Transform.h";

#include "CircleMesh.h";

class Circle :public Model
{
private:
	CircleMesh* circleMesh;

public:
	Circle(Material* material, Texture* texture);

	void render();

	~Circle();
};

