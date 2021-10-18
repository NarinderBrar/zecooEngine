#pragma once

#include "Shader.h"
#include "Material.h"
#include "Camera.h"
#include "Light.h"

#include "Triangle.h"

#include "PhysicsEngine.h"

#include "Input.h"

class SceneBasic
{
	Shader* shader;
	DirectionalLight* light;
	Material* material;

	Camera* camera;
	Triangle* triangle;

public:
	SceneBasic(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input );
	void Update(float deltaTime);

	void Render();
};

