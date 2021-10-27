#include "SceneHelic.h"
SceneHelic::SceneHelic(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input)
{
	phyEng = physicsEngine;

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	helicTexture = new Texture();
	helicTexture->Load("resources\\textures\\bm2.jpg");

	helicGun = new Texture();
	helicGun->Load("resources\\textures\\gold.jpg");

	helicBlade = new Texture();
	helicBlade->Load("resources\\textures\\steel.jpg");

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT);
	camera->SetPerspectiveProjectionMatrix(glm::radians(265.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f);
	camera->Set(cameraPos, looookat, cameraUp);

	shaderG = new Shader("resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs");
	//shaderG = new Shader("resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs");

	grid = new Grid(camera);

	dlight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, -1.0, 0.2));
	dlight->diffuse = glm::vec3(1.0, 1.0, 1.0);
	dlight->ambient = glm::vec3(0.5, 0.5, 0.5);

	glm::vec4 colorG = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	materialG = new Material(shaderG, colorG);
	materialG->linkLight(dlight);
	materialG->linkCamera(camera);

	debugger = new Debugger(camera);

	std::string versionString = std::string((const char*)glGetString(GL_VERSION));
	debugger->printMsg("Darkzy : " + versionString);

	cubeG = new Cube(materialG, helicTexture);
	cubeR = new Cube(materialG, helicTexture);
	cubeH = new Cube(materialG, helicTexture);
	cubeA = new Cube(materialG, helicBlade);
	cubeB = new Cube(materialG, helicBlade);
	cubeC = new Cube(materialG, helicBlade);
	cubeD = new Cube(materialG, helicBlade);
	cubeE = new Cube(materialG, helicBlade);
	cubeF = new Cube(materialG, helicBlade);
	cubeI = new Cube(materialG, helicTexture);
	cubeJ = new Cube(materialG, helicTexture);
	cubeK = new Cube(materialG, helicBlade);
	cubeL = new Cube(materialG, helicBlade);
	cubeM = new Cube(materialG, helicBlade);
	cubeN = new Cube(materialG, helicBlade);
	cubeO = new Cube(materialG, helicTexture);
	cubeP = new Cube(materialG, helicBlade);
	cubeQ = new Cube(materialG, helicBlade);
	cubeS = new Cube(materialG, helicGun);
	cubeT = new Cube(materialG, helicGun);
	cubeU = new Cube(materialG, helicGun);
	cubeV = new Cube(materialG, helicGun);

	//-----------------------------------------//
	cubeR->transform->parent = cubeG->transform;

	//-----------------------------------------//
	cubeH->transform->parent = cubeG->transform;

	//-----------------------------------------//
	cubeA->transform->parent = cubeG->transform;
	cubeB->transform->parent = cubeA->transform;
	cubeC->transform->parent = cubeA->transform;

	//-----------------------------------------//
	cubeD->transform->parent = cubeG->transform;
	cubeE->transform->parent = cubeD->transform;
	cubeF->transform->parent = cubeD->transform;

	//-----------------------------------------//
	cubeI->transform->parent = cubeG->transform;

	//-----------------------------------------//
	cubeJ->transform->parent = cubeG->transform;

	//-----------------------------------------//
	cubeK->transform->parent = cubeJ->transform;
	cubeS->transform->parent = cubeK->transform;

	cubeL->transform->parent = cubeJ->transform;
	cubeT->transform->parent = cubeL->transform;

	cubeM->transform->parent = cubeJ->transform;
	cubeU->transform->parent = cubeM->transform;

	cubeN->transform->parent = cubeJ->transform;
	cubeV->transform->parent = cubeN->transform;

	//-----------------------------------------//
	cubeO->transform->parent = cubeG->transform;
	cubeP->transform->parent = cubeG->transform;
	cubeQ->transform->parent = cubeG->transform;
	//-----------------------------------------//
	
	
	
}

void SceneHelic::RenderObject()
{
	debugger->draw();
	grid->Render();
	cubeG->render();
	cubeR->render();
	cubeH->render();
	cubeA->render();
	cubeB->render();
	cubeC->render();
	cubeD->render();
	cubeE->render();
	cubeF->render();
	cubeI->render();
	cubeJ->render();
	cubeK->render();
	cubeL->render();
	cubeM->render();
	cubeN->render();
	cubeO->render();
	cubeP->render();
	cubeQ->render();
	cubeS->render();
	cubeT->render();
	cubeU->render();
	cubeV->render();
}

void SceneHelic::helicMove(float dt)
{
	const float helicSpeed = 10.0f * dt; // adjust accordingly
	if (GetKeyState('W') & 0x80)
		helicTranslation.z -= 0.1 * helicSpeed;
	if (GetKeyState('S') & 0x80)
		helicTranslation.z += 0.1 * helicSpeed;
	if (GetKeyState('A') & 0x80)
		helicTranslation.x -= 0.1 * helicSpeed;
	if (GetKeyState('D') & 0x80)
		helicTranslation.x += 0.1 * helicSpeed;

	if (GetKeyState(VK_RIGHT) & 0x80)
		cubeG->transform->rotate(glm::radians(50 * dt), glm::vec3(0, 1, 0));

	if (GetKeyState(VK_LEFT) & 0x80)
		cubeG->transform->rotate(glm::radians(-50 * dt), glm::vec3(0, 1, 0));

	if (GetKeyState(VK_UP) & 0x80)
		helicTranslation.y += 0.1 * helicSpeed;

	if (GetKeyState(VK_DOWN) & 0x80)
		helicTranslation.y -= 0.1 * helicSpeed;
}

void SceneHelic::Update(float deltaTime)
{
	helicMove(deltaTime);
	//CameraSettings();
	//camera->Set(cameraPos, looookat, cameraUp);
	//cubeG(CenterBody)


	cubeG->transform->Update();

	//cubeG->transform->rotate(glm::radians(50 * deltaTime), glm::vec3(0, 1, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(helicTranslation));
	cubeG->transform->pose = cubeG->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(1.5, 1.5, 2));
	cubeG->transform->pose = cubeG->transform->pose * I;

	//CubeR (Back Cube 1)
	cubeR->transform->resetParentScale(glm::vec3(1, 1, 1));
	cubeR->transform->Update();

	//angle += deltaTime * angleRotation;
	//swordR->transform->rotate(glm::radians(glm::sin(angle) * deltaTime/3) * 1000, glm::vec3(1, 0, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0, 0.2, 1.5));
	cubeR->transform->pose = cubeR->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.1, 0.21, 2));
	cubeR->transform->pose = cubeR->transform->pose * I;

	//CubeH (Back Cube 2)
	cubeH->transform->resetParentScale(glm::vec3(1, 1, 1));
	cubeH->transform->Update();

	//angle += deltaTime * angleRotation;
	//swordR->transform->rotate(glm::radians(glm::sin(angle) * deltaTime/3) * 1000, glm::vec3(1, 0, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0, 0.4, 2.5));
	cubeH->transform->pose = cubeH->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.1, 0.6, 0.2));
	cubeH->transform->pose = cubeH->transform->pose * I;

	//CubeA (Back Cube rotatater)
	cubeA->transform->resetParentScale(glm::vec3(1, 1, 1));
	cubeA->transform->Update();

	//angle += deltaTime * angleRotation;
	//cubeA->transform->rotate(glm::radians(glm::sin(angle) * deltaTime/3) * 1000, glm::vec3(0, 1, 1));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0, 0.2, 2.5));
	cubeA->transform->pose = cubeA->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.3, 0.05, 0.05));
	cubeA->transform->pose = cubeA->transform->pose * I;

	//CubeB (Back Cube rotatater 1)
	cubeB->transform->resetParentScale(glm::vec3(0.3, 0.05, 0.05));
	cubeB->transform->Update();

	angle += deltaTime * angleRotation;
	cubeB->transform->rotate(glm::radians((-angle) * deltaTime/3) * 1000, glm::vec3(1, 0, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0.2, 0.0, 0));
	cubeB->transform->pose = cubeB->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.05, 0.1, 0.7));
	cubeB->transform->pose = cubeB->transform->pose * I;

	//CubeC (Back Cube rotatater 2)
	cubeC->transform->resetParentScale(glm::vec3(0.3, 0.05, 0.05));
	cubeC->transform->Update();

	angle += deltaTime * angleRotation;
	cubeC->transform->rotate(glm::radians((-angle) * deltaTime/3) * 1000, glm::vec3(1, 0, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(-0.2, 0.0, 0));
	cubeC->transform->pose = cubeC->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.05, 0.1, 0.7));
	cubeC->transform->pose = cubeC->transform->pose * I;

	//CubeD (Top Propeller)
	cubeD->transform->resetParentScale(glm::vec3(1.5, 1.5, 2));
	cubeD->transform->Update();

	angle += deltaTime * angleRotation;
	cubeD->transform->rotate(glm::radians((angle) * deltaTime/3) * 1000, glm::vec3(0, 1, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0, 1, 0));
	cubeD->transform->pose = cubeD->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.1, 0.3, 0.1));
	cubeD->transform->pose = cubeD->transform->pose * I;

	//CubeE (Top Propeller Fan 1)
	cubeE->transform->resetParentScale(glm::vec3(0.1, 0.3, 0.1));
	cubeE->transform->Update();

	//angle += deltaTime * angleRotation;
	//cubeE->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 1, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0, 0.2, 0));
	cubeE->transform->pose = cubeE->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(8, 0.1, 0.15));
	cubeE->transform->pose = cubeE->transform->pose * I;

	//CubeF (Top Propeller Fan 2)
	cubeF->transform->resetParentScale(glm::vec3(0.1, 0.3, 0.1));
	cubeF->transform->Update();

	//angle += deltaTime * angleRotation;
	//cubeE->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 1, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0, 0.2, 0));
	cubeF->transform->pose = cubeF->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.15, 0.1, 8));
	cubeF->transform->pose = cubeF->transform->pose * I;

	//CubeI (Top Front)
	cubeI->transform->resetParentScale(glm::vec3(1.5, 1.5, 2));
	cubeI->transform->Update();

	//angle += deltaTime * angleRotation;
	//cubeI->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 1, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0, -0.25, -1.5));
	cubeI->transform->pose = cubeI->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(1.5, 1, 1));
	cubeI->transform->pose = cubeI->transform->pose * I;

	//CubeJ (SidePannel)
	cubeJ->transform->resetParentScale(glm::vec3(1.5, 1.5, 2));
	cubeJ->transform->Update();

	//angle += deltaTime * angleRotation;
	//cubeJ->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 1, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0, -0.2, 0.4));
	cubeJ->transform->pose = cubeJ->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(5, 0.1, 0.4));
	cubeJ->transform->pose = cubeJ->transform->pose * I;

	//CubeK (Machine Gun 1)
	cubeK->transform->resetParentScale(glm::vec3(5, 0.1, 0.4));
	cubeK->transform->Update();

	//angle += deltaTime * angleRotation;
	//cubeK->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 1, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(2.35, -0.2, -0.2));
	cubeK->transform->pose = cubeK->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.3, 0.3, 1.5));
	cubeK->transform->pose = cubeK->transform->pose * I;

	//CubeL (Machine Gun 2)
	cubeL->transform->resetParentScale(glm::vec3(5, 0.1, 0.4));
	cubeL->transform->Update();

	//angle += deltaTime * angleRotation;
	//cubeL->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 1, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(-2.35, -0.2, -0.2));
	cubeL->transform->pose = cubeL->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.3, 0.3, 1.5));
	cubeL->transform->pose = cubeL->transform->pose * I;

	//CubeM (Machine Gun 3)
	cubeM->transform->resetParentScale(glm::vec3(5, 0.1, 0.4));
	cubeM->transform->Update();

	//angle += deltaTime * angleRotation;
	//cubeL->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 1, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(-1.35, -0.2, -0.4));
	cubeM->transform->pose = cubeM->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.2, 0.2, 1.0));
	cubeM->transform->pose = cubeM->transform->pose * I;

	//CubeN (Machine Gun 4)
	cubeN->transform->resetParentScale(glm::vec3(5, 0.1, 0.4));
	cubeN->transform->Update();

	//angle += deltaTime * angleRotation;
	//cubeL->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 1, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(1.35, -0.2, -0.4));
	cubeN->transform->pose = cubeN->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.2, 0.2, 1.0));
	cubeN->transform->pose = cubeN->transform->pose * I;

	//CubeO (backPanel)
	cubeO->transform->resetParentScale(glm::vec3(1.5, 1.5, 2));
	cubeO->transform->Update();

	//angle += deltaTime * angleRotation;
	//cubeJ->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 1, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0,0.25, 1.5));
	cubeO->transform->pose = cubeO->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(1.5, 1, 1));
	cubeO->transform->pose = cubeO->transform->pose * I;

	//CubeP (sideBooster 1)
	cubeP->transform->resetParentScale(glm::vec3(1.5, 1.5, 2));
	cubeP->transform->Update();

	//angle += deltaTime * angleRotation;
	//cubeJ->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 1, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0.95, 0.5, 1.0));
	cubeP->transform->pose = cubeP->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.4, 0.4, 2));
	cubeP->transform->pose = cubeP->transform->pose * I;

	//CubeP (sideBooster 2)
	cubeQ->transform->resetParentScale(glm::vec3(1.5, 1.5, 2));
	cubeQ->transform->Update();

	//angle += deltaTime * angleRotation;
	//cubeJ->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 1, 0));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(-0.95, 0.5, 1.0));
	cubeQ->transform->pose = cubeQ->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.4, 0.4, 2));
	cubeQ->transform->pose = cubeQ->transform->pose * I;

	//CubeS (Machine Gun 1 rotater)
	cubeS->transform->resetParentScale(glm::vec3(0.3, 0.3, 1.5));
	cubeS->transform->Update();

	angle += deltaTime * angleRotation;
	cubeS->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 0, 1));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0, 0, -0.2));
	cubeS->transform->pose = cubeS->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.1, 0.1, 1.7));
	cubeS->transform->pose = cubeS->transform->pose * I;

	//CubeT (Machine Gun 2 rotater)
	cubeT->transform->resetParentScale(glm::vec3(0.3, 0.3, 1.5));
	cubeT->transform->Update();

	angle += deltaTime * angleRotation;
	cubeT->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 0, 1));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0, 0, -0.2));
	cubeT->transform->pose = cubeT->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.1, 0.1, 1.7));
	cubeT->transform->pose = cubeT->transform->pose * I;

	//CubeU (Machine Gun 3 Rotater)
	cubeU->transform->resetParentScale(glm::vec3(0.2, 0.2, 1.0));
	cubeU->transform->Update();

	angle += deltaTime * angleRotation;
	cubeU->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 0, 1));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0, 0, -0.2));
	cubeU->transform->pose = cubeU->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.1, 0.1, 1.0));
	cubeU->transform->pose = cubeU->transform->pose * I;

	//CubeV (Machine Gun 4 Rotater)
	cubeV->transform->resetParentScale(glm::vec3(0.2, 0.2, 1.0));
	cubeV->transform->Update();

	angle += deltaTime * angleRotation;
	cubeV->transform->rotate(glm::radians((angle)*deltaTime / 3) * 1000, glm::vec3(0, 0, 1));

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0, 0, -0.2));
	cubeV->transform->pose = cubeV->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.1, 0.1, 1.0));
	cubeV->transform->pose = cubeV->transform->pose * I;
}

void SceneHelic::MoveTowards(float dt)
{
	//get new position of cubeR and update in newPos
	newPos = cubeR->transform->getPosition();

	//check distance between CubeHPos and newPos
	float a = glm::distance(CubeHPos, newPos);


	cout << a << endl;

	//if distance is more than 1.0, move toward cube
	if (a > 0.001)
	{
		//calculate move direction, that will be normalized CubeHPos - newPos
		glm::vec3 dir = glm::normalize(CubeHPos - newPos);

		//simply translate cubeR with this direction, multiply dt and any other value for speed
		cubeR->transform->translate(dir * dt * speed);
	}
}

void SceneHelic::DeltaTimeFunction()
{
	float cf = glfwGetTime();
	dt = cf - lf;
	lf = cf;
}

void SceneHelic::camSet2()
{
	//Not In Use
	//------x-------------x-----------x----------------x---------------x-------------x---------------//
	DeltaTimeFunction();
	const float cameraSpeed = 10.0f * dt; // adjust accordingly
	if (GetKeyState('W') & 0x80)
		cameraPos += cameraSpeed * cameraFront;
	if (GetKeyState('S') & 0x80)
		cameraPos -= cameraSpeed * cameraFront;
	if (GetKeyState('A') & 0x80)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (GetKeyState('D') & 0x80)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	//------x-------------x-----------x----------------x---------------x-------------x----------------//
}

void SceneHelic::CameraSettings()
{
	//Movements
	//W-front
	//S-back
	//A-left
	//D-right
	//UpArrow,DownArrow,LeftArrow,RightArrow - rotations



	DeltaTimeFunction();
	//1st Movement
	float cs = 100 * dt;
	float rs = 10 * dt;
	//Xaxis Forward and Backward with W and S Keys
	if (GetKeyState('W') & 0x80)
		looookat.x = looookat.x - 0.1 * cs;

	if (GetKeyState('S') & 0x80)
		looookat.x = looookat.x + 0.1 * cs;

	//Zaxis Left and Right with A and S Keys
	if (GetKeyState('A') & 0x80)
		looookat.z = looookat.z + 0.1 * cs;

	if (GetKeyState('D') & 0x80)
		looookat.z = looookat.z - 0.1 * cs;

	//2ndMovement (Rotation Againt X axis and Z axis)
	if (GetKeyState(VK_RIGHT) & 0x80)
		cameraPos.z -= looookat.y * rs;

	if (GetKeyState(VK_LEFT) & 0x80)
		cameraPos.z += looookat.y * rs;

	if (GetKeyState(VK_UP) & 0x80)
		cameraPos.x -= looookat.y * rs;

	if (GetKeyState(VK_DOWN) & 0x80)
		cameraPos.x += looookat.y * rs;

	//Reset
	if (GetKeyState(0x52) & 0x80)
	{
		cameraPos = glm::vec3(100.0, 100.0, 0.0);
		looookat = glm::vec3(-0.0, 10.0, 00.0);
	}
}

void SceneHelic::BackgroundChange2()
{
	float R1 = 0;
	float G1 = 0;
	float B1 = 0;
	for (int i = 0; i < 1; i++)
	{
		DeltaTimeFunction();
		angle += 0.01 + dt;
		float RGBR = sin(-angle);
		R1 = (RGBR + 1.0f) / 2.0f;
		float RGBG = cos(-angle);
		G1 = (RGBG + 1.0f) / 2.0f;
		float RGBB = tan(-angle);
		B1 = (RGBB + 1.0f) / 2.0f;
		glClearColor(R1, G1, B1, 1.0f);
		i = 0;
	}
}

void SceneHelic::Render()
{
	//BackgroundChange2();
	glClearColor(1, 1, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderObject();
}