#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Plane.h"
#include "Triangle.h"
#include "Cube.h"
#include "TestModel.h"
#include "Grid.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"

#include "Input.h"

class Wave
{
public:
	Texture* texture;
	Texture* texture1;

	Shader* shader;
	Shader* Ashader;

	DirectionalLight* dlight;

	Material* material;
	Material* materialLight;
	Material* testMaterial;

	Plane* plane;
	Cube* cube;
	Cube* cubes[15][15];
	Triangle* triangle1;
	TestModel* testModel;

	PhysicsEngine* phyEng;

	Camera* camera;
	Grid* grid;

	float angle = 0.0;
	float amplitude = 0.5;
	float velocity = 1.0;
	float u_time = 0.0f;
	glm::vec2 u_resolution = glm::vec2(800, 600);

	glm::mat4 projection;

	Wave(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input);

	void sinwave(float deltaTime);

	void calcenter();
	void Update(float deltaTime);

	void Render();

};