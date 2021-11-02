#pragma once
#include <cstdlib>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Grid.h"
#include "Debugger.h"

#include "Cube.h"
#include "EmptyObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"

#include "Input.h"

class SceneHelic
{
public:
	Shader* shaderG;

	DirectionalLight* dlight;

	Material* materialG;

	Texture* helicTexture;
	Texture* helicGun;
	Texture* helicBlade;

	
	Cube* cubeR;
	Cube* cubeA;
	Cube* cubeB;
	Cube* cubeC;
	Cube* cubeD;
	Cube* cubeE;
	Cube* cubeF;
	Cube* cubeG;
	Cube* cubeH;
	Cube* cubeI;
	Cube* cubeJ;
	Cube* cubeK;
	Cube* cubeL;
	Cube* cubeM;
	Cube* cubeN;
	Cube* cubeO;
	Cube* cubeP;
	Cube* cubeQ;
	Cube* cubeS;
	Cube* cubeT;
	Cube* cubeU;
	Cube* cubeV;

	EmptyObject* eo;

	//Variables
	double angle = 0;
	double angleRotation = 0.1;
	float dt = 0.0;
	float lf = 0.0;
	float speed = 10.0;

	glm::mat4 I = glm::mat4(1.0);
	glm::vec3 helicTranslation = glm::vec3(0, 0, 0);

	glm::vec3 looookat = glm::vec3(-0.0, 0.0, 00.0);
	glm::vec3 offsett = glm::vec3(0, 100.0, -100.0);
	glm::vec3 cameraPos = glm::vec3(100.0, 50.0, -50.0);
	glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
	glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
	//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::vec3 camPos2 = cameraPos + cameraFront;

	//-------------------------------------------------//
	glm::vec3 CubeRPos = glm::vec3(0, 0, 0);
	glm::vec3 CubeHPos = glm::vec3(0, 10, 10);
	glm::vec3 newPos = glm::vec3(0, 0, 0);
	//-------------------------------------------------//

	//Debug
	Grid* grid;
	Debugger* debugger;

	PhysicsEngine* phyEng;

	Camera* camera;
	glm::mat4 projection;

	SceneHelic(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input);

	void Update(float deltaTime);
	void helicMove(float deltaTime);
	void DeltaTimeFunction();
	void MoveTowards(float dt);
	void camSet2();
	void BackgroundChange2();
	void CameraSettings();
	void RenderObject();
	void Render();
};

