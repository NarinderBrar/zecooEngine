#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Cube.h"
#include "Cylinder.h"
#include "Grid.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"

class Scene
{
public:
	Shader* shader;

	DirectionalLight* dlight;
	Material* material;

	Texture* floorTexture;

	Cube* cube;
	Grid* grid;

	//Cylinder* cylinder;

	PhysicsEngine* phyEng;

	Camera* camera;
	glm::mat4 projection;

	Scene(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine);

	void Update(float deltaTime);

	void Render();

};