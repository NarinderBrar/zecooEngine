#include "FinalLevel.h"

FinalLevel::FinalLevel(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input)
{
	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	phyEng = physicsEngine;

	input = _input;

	cubeTexture = new Texture();
	cubeTexture->Load("resources\\textures\\brickwall.jpg");
	cubeTexture->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	cubeTexture->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	cubeTexture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	cubeTexture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT);
	camera->SetPerspectiveProjectionMatrix(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 20.0f, 5000.0f);
	glm::vec3 camPos = glm::vec3(0.0, 300.0, 2500.0);
	glm::vec3 camView = glm::vec3(10.0, 3.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
	camera->Set(camPos, camView, camUp);

	LdShader = new Shader("resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs");

	debugDraw = new GLDebugDrawer(camera);
	phyEng->dynamicsWorld->setDebugDrawer(debugDraw);

	LdWallsLight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -1.0, 0)); //0.2, -1.0, 0.2
	LdWallsLight->diffuse = glm::vec3(0.5, 0.5, 0.5);
	LdWallsLight->ambient = glm::vec3(0.9, 0.9, 0.9);

	BackgroundWallsLight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -1.0, 0)); //0.2, -1.0, 0.2
	BackgroundWallsLight->diffuse = glm::vec3(0.5, 0.5, 0.5);
	BackgroundWallsLight->ambient = glm::vec3(0.8, 0.8, 0.8);

	PadLight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -1.0, 0)); //0.2, -1.0, 0.2
	PadLight->diffuse = glm::vec3(0.5, 0.5, 0.5);
	PadLight->ambient = glm::vec3(0.0, 0.0, 0.0);

	glm::vec4 color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	LdMAterial = new Material(LdShader, color);
	LdMAterial->linkLight(LdWallsLight);
	LdMAterial->linkCamera(camera);

	glm::vec4 Backgroundcolor = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	BackgroundMaterial = new Material(LdShader, Backgroundcolor);
	BackgroundMaterial->linkLight(BackgroundWallsLight);
	BackgroundMaterial->linkCamera(camera);

	glm::vec4 PadColor = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	PadMAterials = new Material(LdShader, PadColor);
	PadMAterials->linkLight(PadLight);
	PadMAterials->linkCamera(camera);

	ClearWalls = new Cube(LdMAterial, NULL);
	ClearWalls->transform->translate(glm::vec3(2, -2.3, 0));
	ClearWalls->transform->scale(glm::vec3(22, 1, 7));
	ClearWallsDesign[0] = ClearWalls;

	ClearWalls = new Cube(LdMAterial, NULL);
	ClearWalls->transform->translate(glm::vec3(2, 10, 0));
	ClearWalls->transform->scale(glm::vec3(22, 1, 7));
	ClearWallsDesign[1] = ClearWalls;

	ClearWalls = new Cube(LdMAterial, NULL);
	ClearWalls->transform->translate(glm::vec3(-15.5, 4, 0));
	ClearWalls->transform->scale(glm::vec3(11, 15, 7));
	ClearWallsDesign[2] = ClearWalls;

	ClearWalls = new Cube(LdMAterial, NULL);
	ClearWalls->transform->translate(glm::vec3(28, 4, 0));
	ClearWalls->transform->scale(glm::vec3(11, 15, 7));
	ClearWallsDesign[3] = ClearWalls;

	ClearWalls = new Cube(LdMAterial, NULL);
	ClearWalls->transform->translate(glm::vec3(-9, 8, 0));
	ClearWalls->transform->scale(glm::vec3(3, 5, 7));
	ClearWallsDesign[4] = ClearWalls;

	ClearWalls = new Cube(LdMAterial, NULL);
	ClearWalls->transform->translate(glm::vec3(22, 8.5, 0));
	ClearWalls->transform->scale(glm::vec3(3, 8, 7));
	ClearWallsDesign[5] = ClearWalls;

	LevelDesignCreater();
	//BackGroundCreater();
	LaunchingPads();
	SpawningPlayer();
	MovingObstacleCreater();
}

void FinalLevel::SolvePhysics()
{
	for (int i = 0; i < 22; i++)
	{
		LevelDesign[i]->solve(phyEng);
	}
	/*for (int i = 0; i < 4; i++)
	{
		Background[i]->solve(phyEng);
	}*/

	LaunchingPad->solve(phyEng);
	FinishingPad->solve(phyEng);

	player->solve(phyEng);

	ObsMOverOneDown->solve(phyEng);
	ObsMOverOneUp->solve(phyEng);
}

void FinalLevel::RenderObjects()
{
	for (int i = 0; i < 6; i++)
	{
		ClearWallsDesign[i]->render();
	}
	for (int i = 0; i < 22; i++)
	{
		LevelDesign[i]->render();
	}
	/*for (int i = 0; i < 4; i++)
	{
		Background[i]->render();
	}*/
	LaunchingPad->render();
	FinishingPad->render();

	player->render();

	ObsMOverOneDown->render();
	ObsMOverOneUp->render();
}

void FinalLevel::Update(float deltaTime)
{
	CamFollower();
	SolvePhysics();
	//camera->Update(deltaTime * 0.1);
	PlayerMovement();
	Collisions();
	MovingPlatformRespawn();
	//->RotateViewPoint(800, glfwGetTime());
}

void FinalLevel::Render()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Debugger();
	RenderObjects();
}

void FinalLevel::SpawningPlayer()
{
	player = new Cube(LdMAterial, NULL);
	player->transform->translate(glm::vec3(-6.9, 1, 0));
	player->transform->scale(glm::vec3(0.5, 0.5, 0.5));
	player->mass = 1.0;
	player->SetRigidbody(phyEng);
	player->rigidBody->setUserPointer(player);
	player->name = "player";
}

void FinalLevel::PlayerMovement()
{
	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		player->rigidBody->applyForce(btVector3(10, 0, 0), btVector3(0, 0, 0));
	}

	if (GetKeyState(VK_LEFT) & 0x80)
	{
		player->rigidBody->applyForce(btVector3(-10, 0, 0), btVector3(0, 0, 0));
	}

	if (GetKeyState(VK_SPACE) & 0x80)
	{
		player->rigidBody->applyForce(btVector3(0, 15, 0), btVector3(0, 0, 0));
	}
}

void FinalLevel::RespawnPlayer()
{
	phyEng->dynamicsWorld->removeRigidBody(player->rigidBody);
	player = new Cube(LdMAterial, NULL);
	player->transform->translate(glm::vec3(-6.9, 1, 0));
	player->transform->scale(glm::vec3(0.5, 0.5, 0.5));
	player->mass = 1.0;
	player->SetRigidbody(phyEng);
	player->rigidBody->setUserPointer(player);
	player->name = "player";
}

void FinalLevel::Collisions()
{
	if (playercheck)
	{
		if (phyEng->CollisionTest("ground", "player"))
		{
			cout << "collision";
			RespawnPlayer();
		}
	}
}

void FinalLevel::CamFollower()
{
	glm::vec3 playerPos = player->transform->getPosition();

	glm::vec3 camPos = glm::vec3(0.0 + playerPos.x, 300.0, 800.0);
	glm::vec3 camView = glm::vec3(0.0 + playerPos.x, 3.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
	camera->Set(camPos, camView, camUp);
}

void FinalLevel::Debugger()
{
	phyEng->dynamicsWorld->debugDrawWorld();
	phyEng->dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe);
}

void FinalLevel::LevelDesignCreater()
{
	//BaseGround
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(-5, -2.3f, 0));
	LdWalls->transform->scale(glm::vec3(11, 1, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[0] = LdWalls;

	//LeftWall One
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(-10, 0, 0));
	LdWalls->transform->rotate(0.4, glm::vec3(0, 0, 1));
	LdWalls->transform->scale(glm::vec3(3, 10, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[1] = LdWalls;

	//LeftWall Two
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(-11.1, 8, 0));
	LdWalls->transform->rotate(0.4, glm::vec3(0, 0, 1));
	LdWalls->transform->scale(glm::vec3(3, 10, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[2] = LdWalls;

	//RightWall One
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(22, 0, 0));
	LdWalls->transform->rotate(0.35, glm::vec3(0, 0, -1));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[3] = LdWalls;

	//RightWall One
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(23, 5, 0));
	LdWalls->transform->rotate(0.1, glm::vec3(0, 0, -1));
	LdWalls->transform->scale(glm::vec3(3, 12, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[4] = LdWalls;

	//Roof
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(-5, 10, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(11, 1, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[5] = LdWalls;

	//Roof Two
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(6.2, 10, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(5, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[6] = LdWalls;

	//Base Two
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(6.2, -2.3, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(5, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[7] = LdWalls;

	//Roof Three
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(17.4, 10, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(11, 1, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[8] = LdWalls;

	//Base Three
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(17.4, -2.3, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(11, 1, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[9] = LdWalls;

	//Roof Four Left
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(-1, 12, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[10] = LdWalls;

	//Base Four Left
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(-1, -0.3, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[11] = LdWalls;

	//Roof Five Left
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(-4, 10, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[12] = LdWalls;

	//Base Five Left
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(-4, -2.3, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[13] = LdWalls;

	//Roof Six Left
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(-7, 8, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[14] = LdWalls;

	//Base Six Left
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(-7, -4.3, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[15] = LdWalls;

	//Roof Seven Right
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(13.4, 12, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[16] = LdWalls;

	//Base Seven Right
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(13.4, -0.3, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[17] = LdWalls;

	//Roof Eight Right
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(16.4, 10, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[18] = LdWalls;

	//Base Eight Right
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(16.4, -2.3, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[19] = LdWalls;

	//Roof Nine Right
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(19.4, 8, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[20] = LdWalls;

	//Base Nine Right
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(19.4, -4.3, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[21] = LdWalls;
}

void FinalLevel::MovingObstacleCreater()
{
	ObsMOverOneDown = new Cube(LdMAterial, NULL);
	ObsMOverOneDown->transform->translate(glm::vec3(2.1, 17, 0));
	ObsMOverOneDown->transform->scale(glm::vec3(3, 10, 7));
	ObsMOverOneDown->mass = 100.0;
	ObsMOverOneDown->SetRigidbody(phyEng);
	ObsMOverOneDown->rigidBody->setUserPointer(ObsMOverOneDown);
	ObsMOverOneDown->name = "ground";
	ObsMOverOneDown->rigidBody->setGravity(btVector3(0, -4, 0));

	ObsMOverOneUp = new Cube(LdMAterial, NULL);
	ObsMOverOneUp->transform->translate(glm::vec3(10.3, -9, 0));
	ObsMOverOneUp->transform->scale(glm::vec3(3, 10, 7));
	ObsMOverOneUp->mass = 100.0;
	ObsMOverOneUp->SetRigidbody(phyEng);
	ObsMOverOneUp->rigidBody->setUserPointer(ObsMOverOneUp);
	ObsMOverOneUp->name = "ground";
	ObsMOverOneUp->rigidBody->setGravity(btVector3(0, 4, 0));
}

void FinalLevel::MovingPlatformRespawn()
{
	glm::vec3 ObsMoverDownPos = ObsMOverOneDown->transform->getPosition();
	cout << ObsMoverDownPos.x << endl;
	cout << ObsMoverDownPos.y << endl;
	cout << ObsMoverDownPos.z << endl;
	cout << endl;
	if (ObsMoverDownPos.y < -10)
	{
		phyEng->dynamicsWorld->removeRigidBody(ObsMOverOneDown->rigidBody);
		phyEng->dynamicsWorld->removeRigidBody(ObsMOverOneUp->rigidBody);
		MovingObstacleCreater();
	}
}

void FinalLevel::BackGroundCreater()
{
	//LeftBotton
	BackgroundWalls = new Cube(BackgroundMaterial, NULL);
	BackgroundWalls->transform->translate(glm::vec3(-5, -0.8, -4));
	BackgroundWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	BackgroundWalls->transform->scale(glm::vec3(8, 5, 0.4));
	BackgroundWalls->mass = 0.0;
	BackgroundWalls->SetRigidbody(phyEng);
	BackgroundWalls->rigidBody->setUserPointer(BackgroundWalls);
	Background[0] = BackgroundWalls;

	//RightTop
	BackgroundWalls = new Cube(BackgroundMaterial, NULL);
	BackgroundWalls->transform->translate(glm::vec3(5.7, 5.5, -4));
	BackgroundWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	BackgroundWalls->transform->scale(glm::vec3(8, 5, 0.4));
	BackgroundWalls->mass = 0.0;
	BackgroundWalls->SetRigidbody(phyEng);
	BackgroundWalls->rigidBody->setUserPointer(BackgroundWalls);
	Background[1] = BackgroundWalls;

	//Center Connector Between LB and RT
	BackgroundWalls = new Cube(BackgroundMaterial, NULL);
	BackgroundWalls->transform->translate(glm::vec3(0, 2, -4));
	BackgroundWalls->transform->rotate(0.5, glm::vec3(0, 0, 1));
	BackgroundWalls->transform->scale(glm::vec3(8, 1, 0.4));
	BackgroundWalls->mass = 0.0;
	BackgroundWalls->SetRigidbody(phyEng);
	BackgroundWalls->rigidBody->setUserPointer(BackgroundWalls);
	Background[2] = BackgroundWalls;

	//RB Connector2
	BackgroundWalls = new Cube(BackgroundMaterial, NULL);
	BackgroundWalls->transform->translate(glm::vec3(0, 2.5, -4));
	BackgroundWalls->transform->rotate(0.7, glm::vec3(0, 0, -1));
	BackgroundWalls->transform->scale(glm::vec3(20, 1, 0.4));
	BackgroundWalls->mass = 0.0;
	BackgroundWalls->SetRigidbody(phyEng);
	BackgroundWalls->rigidBody->setUserPointer(BackgroundWalls);
	Background[3] = BackgroundWalls;
}

void FinalLevel::LaunchingPads()
{
	LaunchingPad = new Cube(PadMAterials, NULL);
	LaunchingPad->transform->translate(glm::vec3(-6.9, -1, 0));
	LaunchingPad->transform->scale(glm::vec3(2.5, 1, 2.5));
	LaunchingPad->mass = 0.0;
	LaunchingPad->SetRigidbody(phyEng);
	LaunchingPad->rigidBody->setUserPointer(LaunchingPad);
	LaunchingPad->name = "launch";

	FinishingPad = new Cube(PadMAterials, NULL);
	FinishingPad->transform->translate(glm::vec3(19.4, -1, 0));
	FinishingPad->transform->scale(glm::vec3(2.5, 1, 2.5));
	FinishingPad->mass = 0.0;
	FinishingPad->SetRigidbody(phyEng);
	FinishingPad->rigidBody->setUserPointer(FinishingPad);
	FinishingPad->name = "finish";
}