#include "RobotScene.h"

SceneRobot::SceneRobot(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input)
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
	camera->SetPerspectiveProjectionMatrix(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f);
	glm::vec3 camPos = glm::vec3(300.0, 300.0, 500.0);
	glm::vec3 camView = glm::vec3(0.0, 2.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
	camera->Set(camPos, camView, camUp);

	shaderG = new Shader("resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs");
	shaderB = new Shader("resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs");
	shaderR = new Shader("resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs");
	shaderW = new Shader("resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs");
	shaderZ = new Shader("resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs");
	grid = new Grid(camera);
	dlight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, -1.0, 0.2));
	dlight->diffuse = glm::vec3(1.0, 1.0, 1.0);
	dlight->ambient = glm::vec3(0.5, 0.5, 0.5);

	glm::vec4 colorG = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	materialG = new Material(shaderG, colorG);
	materialG->linkCamera(camera);
	materialG->linkLight(dlight);
	glm::vec4 colorR = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	materialR = new Material(shaderR, colorR);
	materialR->linkCamera(camera);
	materialR->linkLight(dlight);
	glm::vec4 colorB = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	materialB = new Material(shaderB, colorB);
	materialB->linkCamera(camera);
	materialB->linkLight(dlight);
	glm::vec4 colorW = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	materialW = new Material(shaderW, colorW);
	materialW->linkCamera(camera);
	materialW->linkLight(dlight);
	glm::vec4 colorZ = glm::vec4(0.24f, 0.75f, 0.01f, 1.0f);
	materialZ = new Material(shaderZ, colorW);
	materialZ->linkCamera(camera);
	materialZ->linkLight(dlight);

	triangle = new Triangle(materialB, NULL);
	triangle->transform->scale(glm::vec3(0.35f, 0.35f, 0.35f));
	triangle->transform->translate(glm::vec3(0.0f, 4.4f, 0.0f));
	cubeR = new Cube(materialR, NULL);
	cubeR1 = new Cube(materialR, NULL);
	cubeR2 = new Cube(materialR, NULL);
	cubeG = new Cube(materialG, NULL); cubeG1 = new Cube(materialG, NULL); cubeG2 = new Cube(materialG, NULL);
	cubeB = new Cube(materialB, NULL); cubeB1 = new Cube(materialB, NULL); cubeB2 = new Cube(materialB, NULL);
	cubeW = new Cube(materialW, NULL);
	cubeZ = new Cube(materialZ, NULL);
	//cubeR->transform->rotate(45, glm::vec3(0, 1, 0));
	triangle->transform->parent = cubeR->transform;

	//cubeG->transform->translate(glm::vec3(-0.7f, 3.5f, 0.0f));
	//cubeG->transform->rotate(-45, glm::vec3(1, 0, 0));
	//cubeG->transform->translate(glm::vec3(0, 0.2, 0));


	//cubeZ->transform->translate(glm::vec3(0.7f, 3.5f, 0.0f));
	cubeB->transform->parent = cubeR->transform;
	cubeZ->transform->parent = cubeB->transform;
	cubeG->transform->parent = cubeB->transform;
	cubeG1->transform->parent = cubeR1->transform;

	cubeG2->transform->parent = cubeR->transform;

	cubeR1->transform->parent = cubeZ->transform;
	cubeR2->transform->parent = cubeG->transform;
	cubeB1->transform->parent = cubeR1->transform;
	cubeB2->transform->parent = cubeR2->transform;
	//cubeB->transform->translate(glm::vec3(0.0f, 0.5f, 0.0f));
	cubeW->transform->parent = cubeB->transform;
	cubeW->transform->translate(glm::vec3(0.2f, 0.6f, 0.0f));
	cubeG2->transform->translate(glm::vec3(0.0f, -0.5f, 1.0f));

	os = new oscillator();
	//os->SetAngle("x", 45);
	//os->SetDirection("y", cubeG2);
}

void SceneRobot::Update(float deltaTime)
{
	//os->update(deltaTime);
	cubeR->transform->Update();
	//cubeR->transform->rotate(glm::radians(deltaTime * 60), glm::vec3(0, 1, 0));
	//cubeR->transform->translate(glm::vec3(deltaTime, 0, 0));
	//cubeR->transform->rotate(glm::radians(deltaTime)*60,glm::vec3(0,1,0));

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(1.0, 2.0, 1.0));
	cubeR->transform->pose = cubeR->transform->pose * I;
	static float f = 0.0;
	f += deltaTime;
	//cout << f;
	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(sin(f) * 4.5f, 0.5, 0.0));
	cubeR->transform->pose = cubeR->transform->pose * I;

	triangle->transform->resetParentScale(glm::vec3(1.0, 2.0, 1.0));
	triangle->transform->Update();
	triangle->transform->rotate(glm::radians(deltaTime) * 180, glm::vec3(0, 1, 0));


	cubeG2->transform->resetParentScale(glm::vec3(1.0, 2.0, 1.0));
	cubeG2->transform->Update();

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0.5f, 0.0, 2.0));
	cubeG2->transform->pose = cubeG2->transform->pose * I;
	static float tt1 = 0.75f;

	static float lastTime = glfwGetTime();
	static float var = 0, var1 = 0;
	if (glfwGetTime() - lastTime > tt1)
	{
		cout << tt1 << endl;
		lastTime = glfwGetTime();
		tt1 = 6.4f;
		if (cubeG2->transform->parent)
		{
			cubeG2->transform->parent = NULL;
		}
		else {
			cubeG2->transform->parent = cubeR->transform;
		}
	}


	cubeB->transform->resetParentScale(glm::vec3(1.0, 2.0, 1.0));
	cubeB->transform->Update();
	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.5, 0.6, 0.5));
	cubeB->transform->pose = cubeB->transform->pose * I;
	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(1.2f, 0.8f, 0.0f));
	cubeB->transform->pose = cubeB->transform->pose * I;
	static float an = 0;
	cubeZ->transform->resetParentScale(glm::vec3(0.5, 0.6, 0.5));
	cubeZ->transform->Update();
	float z = glm::sin(angle) * deltaTime;

	static float angle = 0.0f; angle += deltaTime;
	cout << glm::radians(sin(deltaTime) * 50.f) << endl;
	if (cubeG2->transform->parent == NULL) z = 0;else {
		z = glm::sin(angle) * deltaTime;
		//	if (z > 0.004) z = 0;
	}
	cubeZ->transform->rotate(glm::radians(z) * 120, glm::vec3(1, 0, 0));
	//cout << glm::radians(sin(f) / 4.0f) * 0.75f << endl;
	cubeG2->transform->rotate(glm::radians(z) * 120, glm::vec3(1, 0, 0));

	an += 0.1;
	if (an > 200.0) an = 0.0;
	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.5, 0.6, 0.5));
	cubeZ->transform->pose = cubeZ->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0.0f, 1.0f, 0.0f));
	//cubeZ->transform->pose = cubeZ->transform->pose * I;

//	cubeZ->transform->rotate(glm::radians(deltaTime) * 60, glm::vec3(1,  0, 0));

	cubeG->transform->resetParentScale(glm::vec3(0.5, 0.6, 0.5));
	cubeG->transform->Update();
	//angle += deltaTime;
	cubeG->transform->rotate(glm::radians(z) * 120, glm::vec3(1, 0, 0));

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.5, 0.6, 0.5));
	cubeG->transform->pose = cubeG->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(-2.4f, 0.0f, 0.0f));
	cubeG->transform->pose = cubeG->transform->pose * I;



	cubeR1->transform->resetParentScale(glm::vec3(0.5, 0.6, 0.5));
	cubeR1->transform->Update();
	//angle += deltaTime;
//	cubeR1->transform->rotate(glm::radians(deltaTime) * 60, glm::vec3(1, 0, 0));

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.5, 2.0, 0.5));
	cubeR1->transform->pose = cubeR1->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(1.0f, 0.31f, 0.0f));
	cubeR1->transform->pose = cubeR1->transform->pose * I;

	cubeR2->transform->resetParentScale(glm::vec3(0.5, 0.6, 0.5));
	cubeR2->transform->Update();
	//angle += deltaTime;
//	cubeR1->transform->rotate(glm::radians(deltaTime) * 60, glm::vec3(1, 0, 0));

	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.5, 2.0, 0.5));
	cubeR2->transform->pose = cubeR2->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(-1.0f, 0.31f, 0.0f));
	cubeR2->transform->pose = cubeR2->transform->pose * I;

	cubeB2->transform->resetParentScale(glm::vec3(0.5, 2.0, 0.5));
	cubeB2->transform->Update();
	//angle += deltaTime;
//	cubeR1->transform->rotate(glm::radians(deltaTime) * 60, glm::vec3(1, 0, 0));
	cubeB2->transform->rotate(glm::radians(deltaTime) * 180, glm::vec3(0, 1, 0));
	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.5, 0.5, 1.5));
	cubeB2->transform->pose = cubeB2->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0.0f, 2.5f, 0.0f));
	cubeB2->transform->pose = cubeB2->transform->pose * I;


	cubeB1->transform->resetParentScale(glm::vec3(0.5, 2.0, 0.5));
	cubeB1->transform->Update();
	//angle += deltaTime;
//	cubeR1->transform->rotate(glm::radians(deltaTime) * 60, glm::vec3(1, 0, 0));
	cubeB1->transform->rotate(glm::radians(deltaTime) * 180, glm::vec3(0, 1, 0));
	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(1.5, 0.5, 0.5));
	cubeB1->transform->pose = cubeB1->transform->pose * I;

	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0.0f, 2.5f, 0.0f));
	cubeB1->transform->pose = cubeB1->transform->pose * I;
	//I = glm::mat4(1.0);
	//I = glm::translate(I, glm::vec3(0.0, 2.0, 0.0));
	//cubeG->transform->pose = cubeG->transform->pose * I;


	//cubeG->transform->resetParentScale(glm::vec3(0.5, 0.6, 0.5));
	//cubeG->transform->Update();
	//cubeG->transform->rotate(glm::radians(deltaTime) * 60, glm::vec3(1, 0, 0));
	//R



	/*cubeB->transform->resetParentScale(glm::vec3(0.2, 1.0, 0.20));
	cubeB->transform->Update();

	cubeB->transform->rotate(glm::radians(deltaTime*300),glm::vec3(0,1,0));


	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.2, 1.0, 0.2));
	cubeB->transform->pose = cubeB->transform->pose * I;


	I = glm::mat4(1.0);
	I = glm::translate(I, glm::vec3(0.0, 0.5, 0.0));
	cubeB->transform->pose = cubeB->transform->pose * I;

	cubeW->transform->resetParentScale(glm::vec3(0.2, 1, 0.2));
	cubeW->transform->Update();
	I = glm::mat4(1.0);
	I = glm::scale(I, glm::vec3(0.6, 0.2, 0.3));
	cubeW->transform->pose = cubeW->transform->pose * I;*/
}

void SceneRobot::Render()
{
	glClearColor(0.0f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	grid->Render();
	cubeG->render();
	//cubeG1->render();
	cubeG2->render();
	cubeR->render();
	//cubeB->render();
	//cubeW->render();
	triangle->render();
	cubeZ->render();
	cubeR1->render(); cubeR2->render();// cubeB1->render(); cubeB2->render();
}
