#include "FinalLevelPlus.h"

FinalLevelPlus::FinalLevelPlus(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input)
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
	glm::vec3 camPos = glm::vec3(0.0, 300.0, 800.0);
	glm::vec3 camView = glm::vec3(0.0, 3.0, 0.0);
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

	LevelDesignCreater();
	//BackGroundCreater();
	LaunchingPads();
	SpawningPlayer();
	MovingPlatform();
}

void FinalLevelPlus::MovingPlatform()
{
	MovingPlatformOne = new Cube(LdMAterial, NULL);
	MovingPlatformOne->transform->translate(glm::vec3(-4, 3, 0));
	MovingPlatformOne->transform->scale(glm::vec3(1, 1, 1));
	MovingPlatformOne->mass = 100.0;
	MovingPlatformOne->SetRigidbody(phyEng);
	MovingPlatformOne->rigidBody->setUserPointer(player);
	MovingPlatformOne->name = "ground";
	//MovingPlatformOne->rigidBody->drag
	MovingPlatformOne->rigidBody->setGravity(btVector3(0, 0, 0));
	//MovingPlatformOne->rigidBody->setCollisionFlags(MovingPlatformOne->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	//MovingPlatformOne->rigidBody->setActivationState(DISABLE_DEACTIVATION);

	MovingPlatformTwo = new Cube(LdMAterial, NULL);
	MovingPlatformTwo->transform->translate(glm::vec3(4, 3, 0));
	MovingPlatformTwo->transform->scale(glm::vec3(1, 1, 1));
	MovingPlatformTwo->mass = 100.0;
	MovingPlatformTwo->SetRigidbody(phyEng);
	MovingPlatformTwo->rigidBody->setUserPointer(player);
	MovingPlatformTwo->name = "ground";
	//MovingPlatformTwo->rigidBody->drag
	MovingPlatformTwo->rigidBody->setGravity(btVector3(0, 0, 0));
	//MovingPlatformTwo->rigidBody->setCollisionFlags(MovingPlatformTwo->rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	//MovingPlatformTwo->rigidBody->setActivationState(DISABLE_DEACTIVATION);

	MovingPlatformThree = new Cube(LdMAterial, NULL);
	MovingPlatformThree->transform->translate(glm::vec3(0, 2, 0));
	MovingPlatformThree->transform->scale(glm::vec3(1, 1, 1));
	MovingPlatformThree->mass = 100.0;
	MovingPlatformThree->SetRigidbody(phyEng);
	MovingPlatformThree->rigidBody->setUserPointer(player);
	MovingPlatformThree->name = "ground";
	//MovingPlatformThree->rigidBody->drag
	MovingPlatformThree->rigidBody->setGravity(btVector3(0, 0, 0));
	//MovingPlatformThree->rigidBody->setCollisionFlags(MovingPlatformThree->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	//MovingPlatformThree->rigidBody->setActivationState(DISABLE_DEACTIVATION);
}

void FinalLevelPlus::MovingPlatformFunctionality()
{
	glm::vec3 PosMP = MovingPlatformOne->transform->getPosition();

	/*if (PosMP.x > 1)
	{
		MovingPlatformOne->rigidBody->setGravity(btVector3(-1, 0, 0));
	}
	if (PosMP.x < -1)
	{
		MovingPlatformOne->rigidBody->setGravity(btVector3(1, 0, 0));
	}*/

	MovingPlatformOne->rigidBody->applyTorque(btVector3(0, 0, 10));
	MovingPlatformTwo->rigidBody->applyTorque(btVector3(0, 0, 10));
	MovingPlatformThree->rigidBody->applyTorque(btVector3(0, 0, 10));
}

void FinalLevelPlus::SolvePhysics()
{
	for (int i = 0; i < 8; i++)
	{
		LevelDesign[i]->solve(phyEng);
	}
	/*for (int i = 0; i < 4; i++)
	{
		Background[i]->solve(phyEng);
	}*/

	MovingPlatformOne->solve(phyEng);
	MovingPlatformTwo->solve(phyEng);
	MovingPlatformThree->solve(phyEng);

	LaunchingPad->solve(phyEng);
	FinishingPad->solve(phyEng);

	player->solve(phyEng);
}

void FinalLevelPlus::RenderObjects()
{
	for (int i = 0; i < 8; i++)
	{
		LevelDesign[i]->render();
	}
	/*for (int i = 0; i < 4; i++)
	{
		Background[i]->render();
	}*/

	MovingPlatformOne->render();
	MovingPlatformTwo->render();
	MovingPlatformThree->render();

	LaunchingPad->render();
	FinishingPad->render();

	player->render();
}

void FinalLevelPlus::Update(float deltaTime)
{
	SolvePhysics();
	PlayerMovement();
	Collisions();
	MovingPlatformFunctionality();
	//->RotateViewPoint(800, glfwGetTime());

	//MovingPlatformOne->rigidBody->applyCentralForce(btVector3(1000 * deltaTime, 0, 0));
}

void FinalLevelPlus::Render()
{
	glClearColor(1.0f, .5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Debugger();
	RenderObjects();
}

void FinalLevelPlus::SpawningPlayer()
{
	player = new Cube(LdMAterial, NULL);
	player->transform->translate(glm::vec3(-7, 1, 0));
	player->transform->scale(glm::vec3(0.5, 0.5, 0.5));
	player->mass = 1.0;
	player->SetRigidbody(phyEng);
	player->rigidBody->setUserPointer(player);
	player->name = "player";
}

void FinalLevelPlus::PlayerMovement()
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

void FinalLevelPlus::RespawnPlayer()
{
	phyEng->dynamicsWorld->removeRigidBody(player->rigidBody);
	player = new Cube(LdMAterial, NULL);
	player->transform->translate(glm::vec3(-7, 1, 0));
	player->transform->scale(glm::vec3(0.5, 0.5, 0.5));
	player->mass = 1.0;
	player->SetRigidbody(phyEng);
	player->rigidBody->setUserPointer(player);
	player->name = "player";
}

void FinalLevelPlus::Collisions()
{
	if (playercheck)
	{
		if (phyEng->CollisionTest("ground", "player"))
		{
			cout << "collision";
			RespawnPlayer();
			//playercheck = false;
		}
	}
}

void FinalLevelPlus::Debugger()
{
	phyEng->dynamicsWorld->debugDrawWorld();
	phyEng->dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe);
}

void FinalLevelPlus::LevelDesignCreater()
{
	//BaseGround
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(0, -2.3f, 0));
	LdWalls->transform->scale(glm::vec3(22, 1, 7));
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
	LdWalls->transform->translate(glm::vec3(10, 0, 0));
	LdWalls->transform->rotate(0.35, glm::vec3(0, 0, -1));
	LdWalls->transform->scale(glm::vec3(3, 7, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[3] = LdWalls;

	//RightWall One
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(11, 5, 0));
	LdWalls->transform->rotate(0.1, glm::vec3(0, 0, -1));
	LdWalls->transform->scale(glm::vec3(3, 12, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[4] = LdWalls;

	//Roof
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(0, 10, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(22, 1, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[5] = LdWalls;

	//Roof
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(0, 8, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(22, 5, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[6] = LdWalls;

	//Roof
	LdWalls = new Cube(LdMAterial, NULL);
	LdWalls->transform->translate(glm::vec3(0, -2, 0));
	LdWalls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	LdWalls->transform->scale(glm::vec3(22, 5, 7));
	LdWalls->mass = 0.0;
	LdWalls->SetRigidbody(phyEng);
	LdWalls->rigidBody->setUserPointer(LdWalls);
	LdWalls->name = "ground";
	LevelDesign[7] = LdWalls;
}

void FinalLevelPlus::BackGroundCreater()
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

void FinalLevelPlus::LaunchingPads()
{
	LaunchingPad = new Cube(PadMAterials, NULL);
	LaunchingPad->transform->translate(glm::vec3(-7, 0.3, 0));
	LaunchingPad->transform->scale(glm::vec3(2.5, 1, 2.5));
	LaunchingPad->mass = 0.0;
	LaunchingPad->SetRigidbody(phyEng);
	LaunchingPad->rigidBody->setUserPointer(LaunchingPad);
	LaunchingPad->name = "launch";

	FinishingPad = new Cube(PadMAterials, NULL);
	FinishingPad->transform->translate(glm::vec3(7, 0.3, 0));
	FinishingPad->transform->scale(glm::vec3(2.5, 1, 2.5));
	FinishingPad->mass = 0.0;
	FinishingPad->SetRigidbody(phyEng);
	FinishingPad->rigidBody->setUserPointer(FinishingPad);
	FinishingPad->name = "finish";
}