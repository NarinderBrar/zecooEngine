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
	shaderR = new Shader( "resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs" );

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

	glm::vec4 colorR = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
	materialR = new Material( shaderR, colorR );
	materialR->linkLight( dlight );
	materialR->linkCamera( camera );

	debugger = new Debugger(camera);

	std::string versionString = std::string((const char*)glGetString(GL_VERSION));
	debugger->printMsg("OpenGl : " + versionString);

	cubeR = new Cube( materialR, NULL );
	cubeG = new Cube(materialG, NULL);
	cubeB = new Cube(materialB, NULL);

	transformations = new Transformations();

	//cube R
	transformations->Rotate( 45, glm::vec3( 0, 1, 0 ) );
	transformations->Add();
	transformations->Translate( glm::vec3(1,0,0));
	transformations->Add();//matrix added to level 1

	//cube G
	transformations->Translate( glm::vec3( 0, 1, 0 ) );
	transformations->Add();//matrix added to level 4
}

void Scene::Update(float deltaTime)
{
	//camera->RotateViewPoint(500, glfwGetTime());
	//projection = camera->GetPerspectiveProjectionMatrix();

	transformations->list[1] = transformations->Rotate( 45, glm::vec3( 0, 1, 0 ) );
	transformations->list[0] *= transformations->Translate( glm::vec3( 1*deltaTime, 0, 0 ) );
	
    cubeR->transform->pose = transformations->getPose( 2 );
	cubeR->transform->pose *= transformations->Scale( glm::vec3(1.0,0.2,1.0));

	transformations->list[2] = glm::translate( glm::vec3( 0, 1, 0 ) );
	cubeG->transform->pose = transformations->getPose( 3 );
}

void Scene::Render()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	debugger->draw();

	grid->Render();

	cubeR->render();
	cubeG->render();
	//cubeB->render();
}