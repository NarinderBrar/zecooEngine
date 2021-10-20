#include "SceneExperiment.h"
SceneExperiment::SceneExperiment(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input )
{
	phyEng = physicsEngine;

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	floorTexture = new Texture();
	floorTexture->Load("resources\\textures\\cylinder.jpg");
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT);
	camera->SetPerspectiveProjectionMatrix(glm::radians(265.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f);

	shaderG = new Shader("resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs");

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

	cubeG = new Cube(materialG, NULL);
	cubeG->transform->translate(looookat);

	cubeR = new Cube(materialG, NULL);
	
	cubeH = new Cube(materialG, NULL);
	cubeH->transform->translate(CubeHPos);
}

void SceneExperiment::Update(float deltaTime)
{
	CameraSettings();
	//camSet2();

	cubeH->transform->translate( glm::vec3( 0, 3* deltaTime, 0 ) );
	///cubeH->transform->rotate( 10 * deltaTime , glm::vec3( 0, 1, 0 ) );

	glm::vec3 dir = glm::normalize(cubeH->transform->getlocalZDir());
	glm::vec3 point = (cubeH->transform->getPosition() + dir * 100.0f);

	//cubeR->transform->position(point);
	//cubeR->transform->Update();


	cameraPos = point + glm::vec3(0,40,0);
	looookat = cubeH->transform->getPosition();

	camera->Set( cameraPos, looookat, cameraUp );

	//cubeR->transform->MoveTowards( deltaTime, cubeH->transform, 5.0, 1.0 );

	//MoveTowards(deltaTime);
}

void SceneExperiment::MoveTowards(float dt)
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
		glm::vec3 dir = glm::normalize( CubeHPos - newPos) ;

		//simply translate cubeR with this direction, multiply dt and any other value for speed
		cubeR->transform->translate( dir * dt * speed);
	}
}

void SceneExperiment::DeltaTimeFunction()
{
	float cf = glfwGetTime();
	dt = cf - lf;
	lf = cf;
}

void SceneExperiment::camSet2()
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

void SceneExperiment::CameraSettings()
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

void SceneExperiment::BackgroundChange2()
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

void SceneExperiment::Render()
{
	BackgroundChange2();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	debugger->draw();
	grid->Render();
	cubeG->render();
	cubeR->render();
	cubeH->render();
}