#include "LongLevelTwo.h"

LongLevelTwo::LongLevelTwo(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input)
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
}

void LongLevelTwo::Update(float deltaTime)
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
	camera->Update(deltaTime * 0.1);
	CamFollow();
	Controll();
	Respawn();
}

void LongLevelTwo::Render()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (int i = 0; i < 7; i++)
	{
		Wall[i]->render();
	}
	for (int i = 0; i < 7; i++)
	{
		Obt[i]->render();
	}
	for (int i = 0; i < 3; i++)
	{
		Box[i]->render();
	}
	LaunchPad->render();
	FinishPad->render();
	player->render();
}

void LongLevelTwo::Player()
{
	player = new Cube(LdMaterial, NULL);
	player->transform->translate(glm::vec3(-5, 0, 0));
	player->transform->scale(glm::vec3(0.5, 0.5, 0.5));
	player->mass = 1.0;
	player->SetRigidbody(phyEng);
	player->rigidBody->setUserPointer(player);
	player->name = "player";
}

void LongLevelTwo::Respawn()
{
	if (phyEng->CollisionTest("ground", "player"))
	{
		phyEng->dynamicsWorld->removeRigidBody(player->rigidBody);
		Player();
	}
}

void LongLevelTwo::Controll()
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

void LongLevelTwo::Pads()
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

void LongLevelTwo::Level()
{
	/*---------------Box---------------*/
	Boxs = new Cube(LdMaterial, NULL);
	Boxs->transform->translate(glm::vec3(-20, 2, 0));
	Boxs->transform->scale(glm::vec3(20, 18, 7));
	Box[0] = Boxs;

	Boxs = new Cube(LdMaterial, NULL);
	Boxs->transform->translate(glm::vec3(34.5, 2, 0));
	Boxs->transform->scale(glm::vec3(20, 18, 7));
	Box[1] = Boxs;

	Boxs = new Cube(LdMaterial, NULL);
	Boxs->transform->translate(glm::vec3(-8.8, 7, 0));
	Boxs->transform->scale(glm::vec3(5, 5, 7));
	Box[2] = Boxs;


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
	Walls->transform->rotate(0.3, glm::vec3(0, 0, -1));
	Walls->transform->scale(glm::vec3(3, 10, 7));
	Walls->mass = 0.0;
	Walls->SetRigidbody(phyEng);
	Walls->rigidBody->setUserPointer(Walls);
	Walls->name = "ground";
	Wall[1] = Walls;

	//LeftWall Middle
	Walls = new Cube(LdMaterial, NULL);
	Walls->transform->translate(glm::vec3(-10.25, 4, 0));
	Walls->transform->rotate(0.1, glm::vec3(0, 0, -1));
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

void LongLevelTwo::Obstacle()
{
	//Bottom
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(5, 0.25, 0));
	Obts->transform->rotate(0.3, glm::vec3(0, 0, 1));
	Obts->transform->scale(glm::vec3(2, 10.85, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[0] = Obts;

	//Middle
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(2.85, 1.5, 0));
	Obts->transform->rotate(0.4, glm::vec3(0, 0, -1));
	Obts->transform->scale(glm::vec3(11, 2, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[1] = Obts;

	//Top
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(10.5, 7, 0));
	Obts->transform->rotate(0.3, glm::vec3(0, 0, -1));
	Obts->transform->scale(glm::vec3(2, 7, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[2] = Obts;

	//BottomStraight
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(14.25, 0, 0));
	Obts->transform->scale(glm::vec3(2, 7, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[3] = Obts;

	//Middle2
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(21, 4, 0));
	Obts->transform->rotate(0.4, glm::vec3(0, 0, -1));
	Obts->transform->scale(glm::vec3(7, 2, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[4] = Obts;

	//Top1
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(11.5, 7.25, 0));
	Obts->transform->rotate(0.4, glm::vec3(0, 0, 1));
	Obts->transform->scale(glm::vec3(8, 2, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[5] = Obts;

	//Top2
	Obts = new Cube(LdMaterial, NULL);
	Obts->transform->translate(glm::vec3(-6, 7, 0));
	Obts->transform->rotate(0.3, glm::vec3(0, 0, -1));
	Obts->transform->scale(glm::vec3(2, 7, 7));
	Obts->mass = 0.0;
	Obts->SetRigidbody(phyEng);
	Obts->rigidBody->setUserPointer(Obts);
	Obts->name = "ground";
	Obt[6] = Obts;
}

void LongLevelTwo::CamFollow()
{
	glm::vec3 playerPos = player->transform->getPosition();

	glm::vec3 camPos = glm::vec3(0.0 + playerPos.x, 300.0, 800.0);
	glm::vec3 camView = glm::vec3(00.0 + playerPos.x, 3.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
	camera->Set(camPos, camView, camUp);
}