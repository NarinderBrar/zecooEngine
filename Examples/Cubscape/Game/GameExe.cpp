#include "GameExe.h"

GameExe::GameExe(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input)
{
	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	phyEng = physicsEngine;

	input = _input;

	floorTexture = new Texture();
	floorTexture->Load("resources\\textures\\yellow1.jpg");

	cubeTexture = new Texture();
	cubeTexture->Load("resources\\textures\\red.jpg");

	spawnTexture = new Texture();
	spawnTexture->Load("resources\\textures\\gradient.jpg");

	platformTexture = new Texture();
	platformTexture->Load("resources\\textures\\blue1.jpg");

	finishTexture = new Texture();
	finishTexture->Load("resources\\textures\\green.jpg");

	prop1Texture = new Texture();
	prop1Texture->Load("resources\\textures\\green.jpg");

	checkPointOneTexture = new Texture();
	checkPointOneTexture->Load("resources\\textures\\purple1.png");

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT);
	camera->SetPerspectiveProjectionMatrix(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f);

	debugDraw = new GLDebugDrawer(camera);
	phyEng->dynamicsWorld->setDebugDrawer(debugDraw);

	cubeShader = new Shader("resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs");

	dlight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, -1.0, 0.2));
	dlight->diffuse = glm::vec3(1.0, 1.0, 1.0);
	dlight->ambient = glm::vec3(0.5, 0.5, 0.5);

	glm::vec4 color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	floorMaterial = new Material(cubeShader, color);
	floorMaterial->linkLight(dlight);
	floorMaterial->linkCamera(camera);

	cubeMaterial = new Material(cubeShader, color);
	cubeMaterial->linkLight(dlight);
	cubeMaterial->linkCamera(camera);

	player = new Cube(cubeMaterial, cubeTexture);
	player->transform->translate(glm::vec3(0.0f, 8.0f, 0.0f));
	player->mass = 1.0;
	player->SetRigidbody(phyEng);
	player->name = "cubePhy";
	player->rigidBody->setUserPointer(player);
	//player->rigidBody->setFriction(0);

	planeSpawn = new Plane(cubeMaterial, spawnTexture);
	planeSpawn->transform->translate(glm::vec3(0, -0.5, 0));
	planeSpawn->transform->scale(glm::vec3(500, 1, 500));

	water = new Cube(cubeMaterial, platformTexture);
	water->transform->translate(glm::vec3(0, -0, 0));
	water->transform->scale(glm::vec3(100, 0, 100));

	CreatingSpawn();
	LevelGenerator();
	CheckPointOne();
	CheckPointTwo();
	CheckPointThree();
	CreatingFinish();
}

void GameExe::Update(float deltaTime)
{
	CameraSettings();
	//Collisions();
	//Constraints();
	PhySolve();
	PlayerControls();
	PlayerBasicProperty(deltaTime);
}

void GameExe::PhySolve()
{
	player->solve(phyEng);
	int l = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			spawn[l]->solve(phyEng);
			l = l + 1;
		}
	}
	l = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
				finish[l]->solve(phyEng);
				l = l + 1;
		}
	}
	l = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			checkPointOne[l]->solve(phyEng);
			l = l + 1;
		}
	}
	l = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			checkPointTwo[l]->solve(phyEng);
			l = l + 1;
		}
	}
	l = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			checkPointThree[l]->solve(phyEng);
			l = l + 1;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		platformOne[i]->solve(phyEng);
	}
	for (int i = 0; i < 5; i++)
	{
		platformTwo[i]->solve(phyEng);
	}
	for (int i = 0; i < 5; i++)
	{
		platformThree[i]->solve(phyEng);
	}
	box->solve(phyEng);
}

void GameExe::Render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//DebuggerCode();
	player->render();
	water->render();
	//planeSpawn->render();
	int l = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			spawn[l]->render();
			l = l + 1;
		}
	}
	if (SpawnFinish == true)
	{
		l = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				finish[l]->render();
				l = l + 1;
			}
		}
	}
	if (SpawnCheckPointOne == true)
	{
		l = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				checkPointOne[l]->render();
				l = l + 1;
			}
		}
	}
	if (SpawnCheckPointTwo == true)
	{
		l = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				checkPointTwo[l]->render();
				l = l + 1;
			}
		}
	}
	if (SpawnCheckPointThree == true)
	{
		l = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				checkPointThree[l]->render();
				l = l + 1;
			}
		}
	}
	if (SpawnPlatformOne == true)
	{
		for (int i = 0; i < 5; i++)
		{
			platformOne[i]->render();
		}
	}
	if (SpawnPlatformTwo == true)
	{
		for (int i = 0; i < 5; i++)
		{
			platformTwo[i]->render();
		}
	}
	if (SpawnPlatformThree == true)
	{
		for (int i = 0; i < 5; i++)
		{
			platformThree[i]->render();
		}
		box->render();
	}
}

void GameExe::Constraints()
{

}

void GameExe::DebuggerCode()
{
	phyEng->dynamicsWorld->debugDrawWorld();
	phyEng->dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe);
	phyEng->dynamicsWorld->debugDrawWorld();
}

void GameExe::CameraSettings()
{
	playerPos = player->transform->getPosition();
	glm::vec3 camFollow = camPos + playerPos;
	camera->Set(camFollow, playerPos, camUp);
	//cout << camFollow.x << endl;
	//camera->RotateViewPoint(800, glfwGetTime());
}

void GameExe::PlayerBasicProperty(float dt)
{
	boxPos = box->transform->getPosition();
	playerPos = player->transform->getPosition();
	cout << " x:" << playerPos.x << " y:" << playerPos.y << " z:" << playerPos.z << endl;
	if (playerPos.y <= -4)
	{
		cout << playerPos.y;
		player = new Cube(cubeMaterial, cubeTexture);
		player->transform->translate(glm::vec3(0.0f, 8.0f, 0.0f));
		player->mass = 1.0;
		player->SetRigidbody(phyEng);
		player->name = "cubePhy";
		player->rigidBody->setUserPointer(player);
		SpawnFinish = false;
	}
	if (playerPos.x >= 1)
	{
		SpawnPlatformOne = true;
	}
	else
	{
		SpawnPlatformOne = false;
	}
	if (playerPos.x >= 8)
	{
		SpawnPlatformTwo = true;
	}
	else
	{
		SpawnPlatformTwo = false;
	}
	if (playerPos.z >= 9)
	{
		SpawnPlatformThree = true;
	}
	else
	{
		SpawnPlatformThree = false;
	}
	if (playerPos.z >= 16 && playerPos.y <= 11)
	{
		BoostCheck = true;
	}
	else
	{
		BoostCheck = false;
	}
	if (playerPos.y <= 7)
	{
		SpawnCheckPointOne = true;
	}
	else
	{
		SpawnCheckPointOne = false;
	}
	if (playerPos.x >= 5)
	{
		SpawnCheckPointTwo = true;
	}
	else
	{
		SpawnCheckPointTwo = false;
	}
	if (playerPos.z >= 7.5)
	{
		SpawnCheckPointThree = true;
	}
	else
	{
		SpawnCheckPointThree = false;
	}
	if (boxPos.y <= 2)
	{
		SpawnFinish = true;
	}
	if (boxPos.y <= 0)
	{
		box = new Cube(cubeMaterial, floorTexture);
		box->transform->translate(glm::vec3(9, 5, 18));
		box->mass = 1.0;
		box->SetRigidbody(phyEng);
		box->name = "box";
		box->rigidBody->setUserPointer(box);
	}
}

void GameExe::PlayerControls()
{
	if (Jumpcheck == true)
	{
		if (input->getPressedKey() == "Space")
		{
			player->rigidBody->applyForce(btVector3(0, 350, 0), btVector3(0, 0, 0));
			Jumpcheck = false;
		}
	}
	else
	{
		if (input->getPressedKey() == "Space1")
		{
			if (phyEng->CollisionTest("cubePhy", "planeSpawn"))
			{
				player->rigidBody->applyForce(btVector3(0, 0, 0), btVector3(0, 0, 0));
				Jumpcheck = true;
			}
		}
	}

	if (GetKeyState(VK_RIGHT) & 0x80)
		player->rigidBody->applyForce(btVector3(0, 0, -10), btVector3(0, 0, 0));
	if (GetKeyState(VK_LEFT) & 0x80)
		player->rigidBody->applyForce(btVector3(0, 0, 10), btVector3(0, 0, 0));
	if (GetKeyState(VK_UP) & 0x80)
		player->rigidBody->applyForce(btVector3(-10, 0, 0), btVector3(0, 0, 0));
	if (GetKeyState(VK_DOWN) & 0x80)
		player->rigidBody->applyForce(btVector3(10, 0, 0), btVector3(0, 0, 0));
	if (BoostCheck == true)
	{
		if (GetKeyState(0x5A) & 0x80)
		{
			player->rigidBody->applyForce(btVector3(0, 30, 0), btVector3(0, 0, 0));
		}
	}
}

void GameExe::CreatingSpawn()
{
	int l = 0;
	int k = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cube = new Cube(cubeMaterial, spawnTexture);
			cube->transform->translate(glm::vec3(-1.5 + k, 0, j-1.5));
			cube->mass = 0.0;
			cube->SetRigidbody(phyEng);
			//cube->rigidBody->setFriction(0);
			cube->name = "planeSpawn";
			cube->rigidBody->setUserPointer(cube);
			cube->rigidBody->setCollisionFlags(cube->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			cube->rigidBody->setActivationState(DISABLE_DEACTIVATION);
			spawn[l] = cube;
			l = l + 1;
		}
		k = k + 1;
	}
}

void GameExe::LevelGenerator()
{
	for (int i = 0; i < 5; i++)
	{
		cube = new Cube(cubeMaterial, floorTexture);
		cube->transform->translate(glm::vec3(2.5+i, 0, 0));
		cube->mass = 1.0;
		cube->SetRigidbody(phyEng);
		//pOne->rigidBody->setFriction(0);
		cube->name = "platformOne";
		cube->rigidBody->setUserPointer(cube);
		cube->rigidBody->setCollisionFlags(cube->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		cube->rigidBody->setActivationState(DISABLE_DEACTIVATION);
		platformOne[i] = cube;
	}
	for (int i = 0; i < 5; i++)
	{
		cube = new Cube(cubeMaterial, floorTexture);
		cube->transform->translate(glm::vec3(9, 0 + i, 2.5 + i));
		cube->mass = 1.0;
		cube->SetRigidbody(phyEng);
		//pOne->rigidBody->setFriction(0);
		cube->name = "platformTwo";
		cube->rigidBody->setUserPointer(cube);
		cube->rigidBody->setCollisionFlags(cube->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		cube->rigidBody->setActivationState(DISABLE_DEACTIVATION);
		platformTwo[i] = cube;
	}
	int h = 0;
	for (int i = 0; i < 5; i++)
	{
		cube = new Cube(cubeMaterial, floorTexture);
		cube->transform->translate(glm::vec3(9 , 4,11.5+i));
		cube->mass = 1.0;
		cube->SetRigidbody(phyEng);
		//pOne->rigidBody->setFriction(0);
		cube->name = "platformOne";
		cube->rigidBody->setUserPointer(cube);
		cube->rigidBody->setCollisionFlags(cube->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		cube->rigidBody->setActivationState(DISABLE_DEACTIVATION);
		platformThree[i] = cube;
	}
	box = new Cube(cubeMaterial, floorTexture);
	box->transform->translate(glm::vec3(9, 5, 18));
	box->mass = 1.0;
	box->SetRigidbody(phyEng);
	box->name = "box";
	box->rigidBody->setUserPointer(box);
}

void GameExe::CheckPointOne()
{
	int l = 0;
	int k = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cube = new Cube(cubeMaterial, checkPointOneTexture);
			cube->transform->translate(glm::vec3(7.5 + k, 0, j - 1.5));
			cube->mass = 0.0;
			cube->SetRigidbody(phyEng);
			//cube->rigidBody->setFriction(0);
			cube->name = "planeSpawn";
			cube->rigidBody->setUserPointer(cube);
			cube->rigidBody->setCollisionFlags(cube->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			cube->rigidBody->setActivationState(DISABLE_DEACTIVATION);
			checkPointOne[l] = cube;
			l = l + 1;
		}
		k = k + 1;
	}
}

void GameExe::CheckPointTwo()
{
	int l = 0;
	int k = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cube = new Cube(cubeMaterial, checkPointOneTexture);
			cube->transform->translate(glm::vec3(7.5 + k, 4, j + 7.5));
			cube->mass = 0.0;
			cube->SetRigidbody(phyEng);
			//cube->rigidBody->setFriction(0);
			cube->name = "planeSpawn";
			cube->rigidBody->setUserPointer(cube);
			cube->rigidBody->setCollisionFlags(cube->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			cube->rigidBody->setActivationState(DISABLE_DEACTIVATION);
			checkPointTwo[l] = cube;
			l = l + 1;
		}
		k = k + 1;
	}
}

void GameExe::CheckPointThree()
{
	int l = 0;
	int k = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cube = new Cube(cubeMaterial, checkPointOneTexture);
			cube->transform->translate(glm::vec3(7.5 + k, 4, j + 16.5));
			cube->mass = 0.0;
			cube->SetRigidbody(phyEng);
			//cube->rigidBody->setFriction(0);
			cube->name = "planeSpawn";
			cube->rigidBody->setUserPointer(cube);
			cube->rigidBody->setCollisionFlags(cube->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			cube->rigidBody->setActivationState(DISABLE_DEACTIVATION);
			checkPointThree[l] = cube;
			l = l + 1;
		}
		k = k + 1;
	}
}

void GameExe::CreatingFinish()
{
	int l = 0;
	int k = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cube = new Cube(cubeMaterial, finishTexture);
			cube->transform->translate(glm::vec3(7.5 + k, 10, j + 16.5));
			cube->mass = 0.0;
			cube->SetRigidbody(phyEng);
			//cube->rigidBody->setFriction(0);
			cube->name = "planeSpawn";
			cube->rigidBody->setUserPointer(cube);
			cube->rigidBody->setCollisionFlags(cube->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			cube->rigidBody->setActivationState(DISABLE_DEACTIVATION);
			finish[l] = cube;
			l = l + 1;
		}
		k = k + 1;
	}
}

void GameExe::Collisions()
{
	if (phyEng->CollisionTest("pTwo", "propOne"))
	{
		SpawnPlatformOne = true;
		cout << "-_-";
	}
}