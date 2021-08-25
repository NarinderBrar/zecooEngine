#pragma once

#include "Shader.h"
#include "Material.h"
#include "Camera.h"
#include "Light.h"
#include "Cube.h"

#include "PhysicsEngine.h"

class SceneBasic
{
	Shader* shader;
	DirectionalLight* light;
	Material* material;

	Camera* camera;
	Cube* cube;

public:
	SceneBasic(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine);
	void Update(float deltaTime);

	void Render();
};

