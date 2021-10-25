#include "SceneA.h"

SceneA::SceneA(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input )
{
	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	phyEng = physicsEngine;

	input = _input;

	ui = new UI();

	floorTexture = new Texture();
	floorTexture->Load("resources\\textures\\brickwall.jpg");
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	cubeTexture = new Texture();
	cubeTexture->Load("resources\\textures\\container.jpg");
	cubeTexture->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	cubeTexture->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	cubeTexture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	cubeTexture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT);
	camera->SetPerspectiveProjectionMatrix(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f);
	glm::vec3 camPos = glm::vec3(0.0, 300.0, 800.0);
	glm::vec3 camView = glm::vec3(0.0, 3.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
	camera->Set(camPos, camView, camUp);

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

	plane = new Plane(floorMaterial, floorTexture);
	plane->transform->translate( glm::vec3( 5.0f, 0.0f, 0.0f ) );
	plane->transform->scale(glm::vec3(5.0f, 5.0f, 5.0f));

    cube = new Cube(cubeMaterial, cubeTexture);
    cube->transform->translate(glm::vec3(5.0f, 0.0f, 0.0f));

	planePhy = new Plane( floorMaterial, floorTexture );
	planePhy->transform->translate( glm::vec3( 1.0f, 0.0f, 0.0f ) );
	planePhy->transform->scale( glm::vec3( 1.0f, 1.0f, 1.0f ) );

	cubePhy = new Cube( cubeMaterial, cubeTexture );
	cubePhy->transform->translate( glm::vec3( 0.0f, 4.0f, 0.0f ) );

	cubePhy->mass = 1.0;
	cubePhy->SetRigidbody( phyEng );
	
	planePhy->mass = 0.0;
	planePhy->SetRigidbody( phyEng );
	
}

void SceneA::Update(float deltaTime)
{
	if( input->getPressedKey() == "Up" )
		std::cout << "up key" << std::endl;

	cubePhy->solve( phyEng );
	planePhy->solve( phyEng );

    //cube->transform->position( glm::vec3( 5.0f, 0.0 + ui->translation, 0.0f ) );
	cube->transform->Update();

	camera->RotateViewPoint( 800, glfwGetTime() );
	projection = camera->GetPerspectiveProjectionMatrix();
}

void SceneA::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	plane->render();
	
    cube->render();

	planePhy->render();

	cubePhy->render();
	
	ui->Render();
}