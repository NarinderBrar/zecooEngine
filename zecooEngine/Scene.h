#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Cube.h"
#include "Plane.h"

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

	Texture* floorTexture;

	Cube* cube;
	Plane* plane;

	PhysicsEngine* phyEng;

	Camera* camera;
	glm::mat4 projection;

	float phi = 0.0;
	float theta = 0.0;

	Scene(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine);

	void Update(float deltaTime);

	void Render();

};