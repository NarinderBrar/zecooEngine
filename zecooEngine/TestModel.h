#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "TestMesh.h"
#include "Transform.h";

class TestModel :public Model
{
private:
	TestMesh* testMesh;

public:
	TestModel(Material* material, Texture* texture);

	void render();

	~TestModel();
};

