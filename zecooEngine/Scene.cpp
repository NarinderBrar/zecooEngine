#include "Scene.h"

Scene::Scene(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine)
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
	glm::vec3 camView = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
	camera->Set(camPos, camView, camUp);

	shaderG = new Shader("resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs");
	shaderB = new Shader( "resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs" );

	grid = new Grid(camera);

	dlight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, -1.0, 0.2));
	dlight->diffuse = glm::vec3(1.0, 1.0, 1.0);
	dlight->ambient = glm::vec3(0.5, 0.5, 0.5);

	glm::vec4 colorG = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	materialG = new Material(shaderG, colorG);
	materialG->linkLight(dlight);
	materialG->linkCamera(camera);

	glm::vec4 colorB = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
	materialB = new Material( shaderB, colorB );
	materialB->linkLight( dlight );
	materialB->linkCamera( camera );

	debugger = new Debugger(camera);

	std::string versionString = std::string((const char*)glGetString(GL_VERSION));
	debugger->printMsg("OpenGl : " + versionString);

	cubeG = new Cube(materialG, NULL);
	cubeB = new Cube(materialB, NULL);

	matrixStack.Push();

	//Stack for Cube B
	matrixStack.Translate( 2, 0, 0 );
	matrixStack.Push();

	matrixStack.Rotate(glm::vec3(0,1,0),45.0);
	matrixStack.Push();

	matrixStack.Scale(1,0.2,1);
	matrixStack.Push();

	cubeB->transform->pose = matrixStack.Top();

	matrixStack.Pop();
	matrixStack.Pop();
	matrixStack.Pop();

	//Stack for Cube G
	matrixStack.Translate( 0, 0.5, 0 );
	matrixStack.Push();

	matrixStack.Scale( 0.2, 1, 0.2 );
	matrixStack.Push();

	cubeG->transform->pose = matrixStack.Top();
}

void Scene::Update(float deltaTime)
{
	//camera->RotateViewPoint(500, glfwGetTime());
	//projection = camera->GetPerspectiveProjectionMatrix();
}

void Scene::Render()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	debugger->draw();

	grid->Render();

	cubeG->render();
	cubeB->render();
}