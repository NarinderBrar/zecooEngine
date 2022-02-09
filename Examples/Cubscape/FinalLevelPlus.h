#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Plane.h"
#include "Cube.h"
#include "Sphere.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"
#include "GLDebugDrawer.h"

#include "Input.h"
class FinalLevelPlus
{
public:
	Texture* cubeTexture;

	Shader* LdShader;

	DirectionalLight* LdWallsLight;
	DirectionalLight* BackgroundWallsLight;
	DirectionalLight* PadLight;

	Material* LdMAterial;
	Material* BackgroundMaterial;
	Material* PadMAterials;

	Cube* player;

	Cube* MovingPlatformOne;
	Cube* MovingPlatformTwo;
	Cube* MovingPlatformThree;

	Cube* LdWalls;
	Cube* LevelDesign[10];

	Cube* BackgroundWalls;
	Cube* Background[10];

	Cube* LaunchingPad;
	Cube* FinishingPad;

	PhysicsEngine* phyEng;
	GLDebugDrawer* debugDraw;

	Input* input;

	Camera* camera;

	float angle = 0.0;
	bool playercheck = true;
	bool rightCheck = false;
	bool leftCheck = false;

	FinalLevelPlus(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input);

	void LevelDesignCreater();
	void BackGroundCreater();
	void LaunchingPads();
	void SpawningPlayer();

	void Update(float deltaTime);

	void Render();

	void RespawnPlayer();
	void Collisions();
	void PlayerMovement();
	void RenderObjects();
	void SolvePhysics();
	void Debugger();
	void MovingPlatform();
	void MovingPlatformFunctionality();
	void MPRespawnRight();
	void MPRespawnLeft();
};



