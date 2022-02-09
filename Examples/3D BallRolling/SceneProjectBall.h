#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"
#include "GLDebugDrawer.h"

#include "Input.h"

class SceneProjectBall
{
public:
	Texture* floorTexture;

	Shader* floorShader;
	Shader* cubeShader;

	DirectionalLight* dlight;

	SpotLight* slight;

	Material* floorMaterial;
	Material* cubeMaterial;

	glm::vec3 playerpos;
	glm::vec3 camPos = glm::vec3(250.0, 300.0, 800.0);
	glm::vec3 camView = glm::vec3(0.0, 3.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);

	Cube* floor;
	Cube* floors[5];
	Cube* floor1;
	Cube* floors1[5];
	Cube* floor2;
	Cube* floors2[5];
	Cube* floor3;
	Cube* floors3[10];
	Sphere* ball;

	PhysicsEngine* phyEng;
	GLDebugDrawer* debugDraw;

	Input* input;

	Camera* camera;

	SceneProjectBall(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input);
	void Update(float deltaTime);

	void Render();
	void camerafollow();

};

