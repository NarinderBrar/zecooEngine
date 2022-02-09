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
class GameExe
{
public:
	Texture* floorTexture;
	Texture* cubeTexture;
	Texture* spawnTexture;
	Texture* platformTexture;
	Texture* finishTexture;
	Texture* prop1Texture;
	Texture* checkPointOneTexture;

	Shader* floorShader;
	Shader* cubeShader;

	DirectionalLight* dlight;

	GLDebugDrawer* debugDraw;
	SpotLight* slight;

	Material* floorMaterial;
	Material* cubeMaterial;

	Cube* cube;
	Cube* spawn[16];
	Cube* finish[16];
	Cube* checkPointOne[16];
	Cube* checkPointTwo[16];
	Cube* checkPointThree[16];
	Cube* platformOne[5];
	Cube* platformTwo[10];
	Cube* platformThree[5];
	Cube* box;
	Cube* pOne;
	Cube* pTwo;
	Cube* propOne;
	Cube* water;

	Cube* ObstacleOne;
	Cube* ObstacleOneProp;

	Cube* player;

	Plane* planeSpawn;
	Plane* platform1;

	PhysicsEngine* phyEng;

	Input* input;

	Camera* camera;

	//Variables
	boolean Jumpcheck = false;
	boolean SpawnPlatformOne = false;
	boolean SpawnPlatformTwo = false;
	boolean SpawnPlatformThree = false;
	boolean BoostCheck = false;
	boolean SpawnCheckPointOne = false;
	boolean SpawnCheckPointTwo = false;
	boolean SpawnCheckPointThree = false;
	boolean SpawnFinish = false;

	glm::vec3 playerPos;
	glm::vec3 boxPos;
	glm::vec3 camPos = glm::vec3(600.0, 600.0, 600.0);
	glm::vec3 camView = glm::vec3(0.0, 3.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);

	GameExe(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input);
	void Update(float deltaTime);

	void Render();
	void DebuggerCode();
	void PhySolve();
	void CameraSettings();
	void PlayerBasicProperty(float deltaTime);
	void PlayerControls();
	void CreatingSpawn();
	void LevelGenerator();
	void Collisions();
	void Constraints();
	void CheckPointOne();
	void CheckPointTwo();
	void CheckPointThree();
	void CreatingFinish();
};

