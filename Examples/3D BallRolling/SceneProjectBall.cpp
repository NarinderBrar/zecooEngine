#include "SceneProjectBall.h"

SceneProjectBall::SceneProjectBall(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input)
{
	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	phyEng = physicsEngine;

	input = _input;

	floorTexture = new Texture();
	floorTexture->Load("resources\\textures\\floor.jpg");
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT);
	camera->SetPerspectiveProjectionMatrix(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f);
	camera->Set(camPos, camView, camUp);

	cubeShader = new Shader("resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs");

	debugDraw = new GLDebugDrawer(camera);
	phyEng->dynamicsWorld->setDebugDrawer(debugDraw);

	dlight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, -1.0, 0.2));
	dlight->diffuse = glm::vec3(1.0, 1.0, 1.0);
	dlight->ambient = glm::vec3(0.5, 0.5, 0.5);

	glm::vec4 color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	floorMaterial = new Material(cubeShader, color);
	floorMaterial->linkLight(dlight);
	floorMaterial->linkCamera(camera);

	/*------------------Floor------------------*/
	/*------------Floor1------------*/
	for (int i = 0; i < 5; i++)
	{
		floor = new Cube(floorMaterial, floorTexture);
		floor->transform->translate(glm::vec3(0 + i, 0, 0 ));
		floor->mass = 1.0;
		floor->SetRigidbody(phyEng);
		floor->rigidBody->setUserPointer(floor);
		floor->rigidBody->setCollisionFlags(floor->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		floor->rigidBody->setActivationState(DISABLE_DEACTIVATION);
		floors[i] = floor;
	}
	/*------------Floor2------------*/
	for (int i = 0; i < 5; i++)
	{
		floor1 = new Cube(floorMaterial, floorTexture);
		floor1->transform->translate(glm::vec3(5.0f, 0, 0.5f + i));
		floor1->mass = 1.0;
		floor1->SetRigidbody(phyEng);
		floor1->rigidBody->setUserPointer(floor);
		floor1->rigidBody->setCollisionFlags(floor->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		floor1->rigidBody->setActivationState(DISABLE_DEACTIVATION);
		floors1[i] = floor1;
	}
	/*------------Floor3------------*/
	for (int i = 0; i < 5; i++)
	{
		floor2 = new Cube(floorMaterial, floorTexture);
		floor2->transform->translate(glm::vec3(6.0f + i, 0, 6.0f));
		floor2->mass = 1.0;
		floor2->SetRigidbody(phyEng);
		floor2->rigidBody->setUserPointer(floor);
		floor2->rigidBody->setCollisionFlags(floor->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		floor2->rigidBody->setActivationState(DISABLE_DEACTIVATION);
		floors2[i] = floor2;
	}
	/*------------Floor4------------*/
	for (int i = 0; i < 10; i++)
	{
		floor3 = new Cube(floorMaterial, floorTexture);
		floor3->transform->translate(glm::vec3(11.0f, 0, 8.0f + i));
		floor3->mass = 1.0;
		floor3->SetRigidbody(phyEng);
		floor3->rigidBody->setUserPointer(floor);
		floor3->rigidBody->setCollisionFlags(floor->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		floor3->rigidBody->setActivationState(DISABLE_DEACTIVATION);
		floors3[i] = floor3;
	}

	/*------------------Ball------------------*/
	ball = new Sphere(floorMaterial, NULL);
	ball->transform->translate(glm::vec3(0.0f, 5.0f, 0.0f));
	ball->mass = 1.0;
	ball->SetRigidbody(phyEng);
}

void SceneProjectBall::Update(float deltaTime)
{
	if (input->getPressedKey() == "Up")
	{
		ball->rigidBody->applyForce(btVector3(0, 0, -3), btVector3(0, 0, 0));
	}
	if (input->getPressedKey() == "Down")
	{
		ball->rigidBody->applyForce(btVector3(0, 0, 3), btVector3(0, 0, 0));
	}
	if (input->getPressedKey() == "Right")
	{
		ball->rigidBody->applyForce(btVector3(3, 0, 0), btVector3(0, 0, 0));
	}
	if (input->getPressedKey() == "Left")
	{
		ball->rigidBody->applyForce(btVector3(-3, 0, 0), btVector3(0, 0, 0));
	}
	if (input->getPressedKey() == "Space")
	{
		ball->rigidBody->applyForce(btVector3(0, 75, 0), btVector3(0, 0, 0));
	}
	
	for (int i = 0; i < 5; i++)
	{
		floors[i]->solve(phyEng);
	}

	for (int i = 0; i < 5; i++)
	{
		floors1[i]->solve(phyEng);
	}

	for (int i = 0; i < 5; i++)
	{
		floors2[i]->solve(phyEng);
	}

	for (int i = 0; i < 10; i++)
	{
		floors3[i]->solve(phyEng);
	}

	ball->solve( phyEng );
	camerafollow();
	//camera->RotateViewPoint(800, glfwGetTime());
}

void SceneProjectBall::camerafollow()
{
	playerpos = ball->transform->getPosition();
	glm::vec3 camfol = camPos + playerpos;
	camera->Set(camfol, playerpos, camUp);
}

void SceneProjectBall::Render()
{
	glClearColor(0.4f, 0.6f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < 5; i++)
	{
		floors[i]->render();
	}

	for (int i = 0; i < 5; i++)
	{
		floors1[i]->render();
	}

	for (int i = 0; i < 5; i++)
	{
		floors2[i]->render();
	}

	for (int i = 0; i < 10; i++)
	{
		floors3[i]->render();
	}

	ball->render();
}