#include "LongLevelThree.h"

LongLevelThree::LongLevelThree(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input)
{
	phyEng = physicsEngine;

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

	WallsLight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -1.0, 0)); //0.2, -1.0, 0.2
	WallsLight->diffuse = glm::vec3(0.5, 0.5, 0.5);
	WallsLight->ambient = glm::vec3(0.9, 0.9, 0.9);

	BackgroundWallsLight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -1.0, 0)); //0.2, -1.0, 0.2
	BackgroundWallsLight->diffuse = glm::vec3(0.5, 0.5, 0.5);
	BackgroundWallsLight->ambient = glm::vec3(0.8, 0.8, 0.8);

	PadLight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -1.0, 0)); //0.2, -1.0, 0.2
	PadLight->diffuse = glm::vec3(0.5, 0.5, 0.5);
	PadLight->ambient = glm::vec3(0.0, 0.0, 0.0);

	glm::vec4 color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	LdMaterial = new Material(LdShader, color);
	LdMaterial->linkLight(WallsLight);
	LdMaterial->linkCamera(camera);

	glm::vec4 Backgroundcolor = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	BackgroundMaterial = new Material(LdShader, Backgroundcolor);
	BackgroundMaterial->linkLight(BackgroundWallsLight);
	BackgroundMaterial->linkCamera(camera);

	glm::vec4 PadColor = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	PadMaterials = new Material(LdShader, PadColor);
	PadMaterials->linkLight(PadLight);
	PadMaterials->linkCamera(camera);

	Level();
	Pads();
	Obstacle();
	Player();
	//PlayerTwo();
	//PlayerThree();
}

void LongLevelThree::Update(float deltaTime)
{
	//camera->RotateViewPoint(1200, glfwGetTime());
	//projection = camera->GetPerspectiveProjectionMatrix();

	for (int i = 0; i < 7; i++)
	{
		Wall[i]->solve(phyEng);
	}
	for (int i = 0; i < 7; i++)
	{
		Obt[i]->solve(phyEng);
	}
	LaunchPad->solve(phyEng);
	FinishPad->solve(phyEng);
	player->solve(phyEng);
	//camera->Update(deltaTime * 0.1);
	CamFollow();
	Respawn();
	Controll();
}

void LongLevelThree::Render()
{
	glClearColor(1.0f, 0.5f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (int i = 0; i < 7; i++)
	{
		Wall[i]->render();
	}
	for (int i = 0; i < 7; i++)
	{
		Obt[i]->render();
	}
	for (int i = 0; i < 2; i++)
	{
		Box[i]->render();
	}
	LaunchPad->render();
	FinishPad->render();
	player->render();
}

void LongLevelThree::Player()
{
	player = new Cube(LdMaterial, NULL);
	player->transform->translate(glm::vec3(-5, 0, 0));
	player->transform->scale(glm::vec3(0.5, 0.5, 0.5));
	player->mass = 1.0;
	player->SetRigidbody(phyEng);
	player->rigidBody->setUserPointer(player);
	player->name = "player";
}

void LongLevelThree::PlayerTwo()
{
	player = new Cube(LdMaterial, NULL);
	player->transform->translate(glm::vec3(-5, 0, 0));
	player->transform->scale(glm::vec3(0.5, 0.5, 0.5));
	player->mass = 1.0;
	player->SetRigidbody(phyEng);
	player->rigidBody->setUserPointer(player);
	player->name = "playertwo";
}

void LongLevelThree::PlayerThree()
{
	player = new Cube(LdMaterial, NULL);
	player->transform->translate(glm::vec3(-5, 0, 0));
	player->transform->scale(glm::vec3(0.5, 0.5, 0.5));
	player->mass = 1.0;
	player->SetRigidbody(phyEng);
	player->rigidBody->setUserPointer(player);
	player->name = "playerthree";
}

void LongLevelThree::Controll()
{
	if (PlayerMovementEnableCheck)
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
}

void LongLevelThree::CamFollow()
{
	glm::vec3 playerPos = player->transform->getPosition();

	glm::vec3 camPos = glm::vec3(0.0 + playerPos.x, 300.0, 800.0);
	glm::vec3 camView = glm::vec3(0.0 + playerPos.x, 3.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
	camera->Set(camPos, camView, camUp);
}

void LongLevelThree::PlayerStop()
{
	player = new Cube(LdMaterial, NULL);
	player->transform->translate(glm::vec3(-6, -1, 0));
	player->transform->scale(glm::vec3(0.3, 0.3, 0.3));
	player->mass = 1.0;
	player->SetRigidbody(phyEng);
	player->rigidBody->setUserPointer(player);
	player->name = "playerStop";
}

void LongLevelThree::Respawn()
{
	if (phyEng->CollisionTest("ground", "player"))
	{
		phyEng->dynamicsWorld->removeRigidBody(player->rigidBody);
		Player();
	}
}

void LongLevelThree::Level()
{
	/*---------------Box---------------*/
	Boxs = new Cube(LdMaterial, NULL);
	Boxs->transform->translate(glm::vec3(-13, 2, 0));
	Boxs->transform->scale(glm::vec3(10, 18, 7));
	Box[0] = Boxs;

	Boxs = new Cube(LdMaterial, NULL);
	Boxs->transform->translate(glm::vec3(29, 1.85, 0));
	Boxs->transform->scale(glm::vec3(10, 18, 7));
	Box[1] = Boxs;

	//Floor
	Walls = new Cube(LdMaterial, NULL);
	Walls->transform->translate(glm::vec3(7, -4.5, 0));
	Walls->transform->scale(glm::vec3(42, 5, 7));
	Walls->mass = 0.0;
	Walls->SetRigidbody(phyEng);
	Walls->rigidBody->setUserPointer(Walls);
	Walls->name = "ground";
	Wall[0] = Walls;

	//LeftWall Bottom
	Walls = new Cube(LdMaterial, NULL);
	Walls->transform->translate(glm::vec3(-10.25, 0, 0));
	Walls->transform->rotate(0.7, glm::vec3(0, 0, 1));
	Walls->transform->scale(glm::vec3(3, 10, 7));
	Walls->mass = 0.0;
	Walls->SetRigidbody(phyEng);
	Walls->rigidBody->setUserPointer(Walls);
	Walls->name = "ground";
	Wall[1] = Walls;

	//LeftWall Middle
	Walls = new Cube(LdMaterial, NULL);
	Walls->transform->translate(glm::vec3(-9.15, 8, 0));
	Walls->transform->rotate(0.5, glm::vec3(0, 0, -1));
	Walls->transform->scale(glm::vec3(3, 5, 7));
	Walls->mass = 0.0;
	Walls->SetRigidbody(phyEng);
	Walls->rigidBody->setUserPointer(Walls);
	Walls->name = "ground";
	Wall[2] = Walls;

	//RightWall Bottom
	Walls = new Cube(LdMaterial, NULL);
	Walls->transform->translate(glm::vec3(24, 0, 0));
	Walls->transform->rotate(0.35, glm::vec3(0, 0, -1));
	Walls->transform->scale(glm::vec3(3, 7, 7));
	Walls->mass = 0.0;
	Walls->SetRigidbody(phyEng);
	Walls->rigidBody->setUserPointer(Walls);
	Walls->name = "ground";
	Wall[3] = Walls;

	//RightWall Top
	Walls = new Cube(LdMaterial, NULL);
	Walls->transform->translate(glm::vec3(25, 5, 0));
	Walls->transform->rotate(0.1, glm::vec3(0, 0, -1));
	Walls->transform->scale(glm::vec3(3, 12, 7));
	Walls->mass = 0.0;
	Walls->SetRigidbody(phyEng);
	Walls->rigidBody->setUserPointer(Walls);
	Walls->name = "ground";
	Wall[4] = Walls;

	//Roof
	Walls = new Cube(LdMaterial, NULL);
	Walls->transform->translate(glm::vec3(7, 12, 0));
	Walls->transform->rotate(0.2, glm::vec3(0, 0, 0));
	Walls->transform->scale(glm::vec3(42, 5, 7));
	Walls->mass = 0.0;
	Walls->SetRigidbody(phyEng);
	Walls->rigidBody->setUserPointer(Walls);
	Walls->name = "ground";
	Wall[5] = Walls;

	//LeftWall Top
	Walls = new Cube(LdMaterial, NULL);
	Walls->transform->translate(glm::vec3(-11, 7, 0));
	Walls->transform->scale(glm::vec3(3, 5, 7));
	Walls->mass = 0.0;
	Walls->SetRigidbody(phyEng);
	Walls->rigidBody->setUserPointer(Walls);
	Walls->name = "ground";
	Wall[6] = Walls;
}

void LongLevelThree::Obstacle()
{
	//BottomTiltOne
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(0.85, -0.85, 0));
	Obts->transform->rotate(0.8, glm::vec3(0, 0, -1));
	Obts->transform->scale(glm::vec3(4, 7, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[0] = Obts;

	//BottomStraight
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(2.85, 0.25, 0));
	//Obts->transform->rotate(0.3, glm::vec3(0, 0, 1));
	Obts->transform->scale(glm::vec3(4.5, 7, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[1] = Obts;

	//Middle
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(6.5, 3.85, 0));
	Obts->transform->rotate(0.4, glm::vec3(0, 0, 1));
	Obts->transform->scale(glm::vec3(8.25, 1.5, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[2] = Obts;

	//TopTilt
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(16.85, 6, 0));
	Obts->transform->rotate(0.825, glm::vec3(0, 0, -1));
	Obts->transform->scale(glm::vec3(2, 13, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[3] = Obts;

	//BottomStraight
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(0, 3.85, 0));
	Obts->transform->rotate(0.4, glm::vec3(0, 0, -1));
	Obts->transform->scale(glm::vec3(8.25, 1.5, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[4] = Obts;

	//Middle2
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(16, -2, 0));
	Obts->transform->rotate(0.4, glm::vec3(0, 0, -1));
	Obts->transform->scale(glm::vec3(2, 4, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[5] = Obts;

	//Top2
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(-6, 9.85, 0));
	Obts->transform->rotate(0.5, glm::vec3(0, 0, 1));
	Obts->transform->scale(glm::vec3(7, 2, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[6] = Obts;
}

void LongLevelThree::Pads()
{
	LaunchPad = new Cube(PadMaterials, NULL);
	LaunchPad->transform->translate(glm::vec3(-5, -2, 0));
	LaunchPad->transform->scale(glm::vec3(2.5, 1, 2.5));
	LaunchPad->mass = 0.0;
	LaunchPad->SetRigidbody(phyEng);
	LaunchPad->rigidBody->setUserPointer(LaunchPad);
	LaunchPad->name = "launch";

	FinishPad = new Cube(PadMaterials, NULL);
	FinishPad->transform->translate(glm::vec3(19, -2, 0));
	FinishPad->transform->scale(glm::vec3(2.5, 1, 2.5));
	FinishPad->mass = 0.0;
	FinishPad->SetRigidbody(phyEng);
	FinishPad->rigidBody->setUserPointer(FinishPad);
	FinishPad->name = "finish";
}