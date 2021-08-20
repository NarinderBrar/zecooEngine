#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Plane.h"
#include "Cube.h"
#include "Triangle.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"

class Scene
{
public:
	Shader* shader;
	Shader* shader2;

	DirectionalLight* dlight;
	Material* material;
	Material* material2;

	Plane* plane;
	Cube* cube;
	Cube* cubes[2];

	Triangle* triangle;

	PhysicsEngine* phyEng;

	Camera* camera;
	glm::mat4 projection;

	Scene(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine);

	void Update(float deltaTime);

	void Render();

};