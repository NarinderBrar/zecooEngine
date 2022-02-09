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
class LongLevelTwo
{
public:
	Texture* cubeTexture;

	Shader* LdShader;

	DirectionalLight* WallsLight;
	DirectionalLight* BackgroundWallsLight;
	DirectionalLight* PadLight;

	Material* LdMaterial;
	Material* BackgroundMaterial;
	Material* PadMaterials;

	Cube* Walls;
	Cube* Wall[10];

	Cube* Boxs;
	Cube* Box[10];

	Cube* player;

	Cube* Obts;
	Cube* Obt[10];

	Cube* LaunchPad;
	Cube* FinishPad;

	PhysicsEngine* phyEng;
	GLDebugDrawer* debugDraw;

	Input* input;

	Camera* camera;

	float k = 0.0;

	LongLevelTwo(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input);

	void Update(float deltaTime);

	void Render();
	void Level();
	void Pads();
	void Obstacle();
	void Player();
	void Respawn();
	void Controll();
	void CamFollow();
};

