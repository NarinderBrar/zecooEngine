#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "CustomMesh.h";
#include "Transform.h";

class CustomModel :public Model
{
private:
	CustomMesh* customMesh;

public:
	CustomModel(Material* material, Texture* texture);

	void render();

	~CustomModel();
};

