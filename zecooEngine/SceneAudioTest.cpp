#include "SceneAudioTest.h"


SceneAudioTest::SceneAudioTest(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input)
{
	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	phyEng = physicsEngine;

	input = _input;

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

	debugDraw = new GLDebugDrawer( camera );
	phyEng->dynamicsWorld->setDebugDrawer( debugDraw );

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

	cubePhy = new Cube(cubeMaterial, cubeTexture);
	cubePhy->transform->translate(glm::vec3(0.0f, 15.0f, 0.0f));
	cubePhy->transform->scale( glm::vec3( 1.0f, 2.0f, 1.0f ) );
	cubePhy->transform->rotate(45,glm::vec3( 0.0f, 1.0f, 0.0f ) );
	cubePhy->mass = 1.0;
	cubePhy->SetRigidbody(phyEng);
	cubePhy->name = "cubePhy";
	cubePhy->rigidBody->setUserPointer(cubePhy);

	cube = new Cube( cubeMaterial, cubeTexture );
	cube->transform->translate( glm::vec3( 0.0f, 10.0f, 0.0f ) );
	cube->transform->scale( glm::vec3( 2.0f, 1.0f, 1.0f ) );
	cube->transform->rotate( 45, glm::vec3( 0.0f, 1.0f, 0.0f ) );
	cube->mass = 1.0;
	cube->SetRigidbody( phyEng );
	cube->name = "cubePhy";
	cube->rigidBody->setUserPointer( cube );

	planePhy = new Plane(floorMaterial, floorTexture);
	planePhy->transform->translate( glm::vec3( 0.0f, 1.0f, 0.0f ) );
	planePhy->transform->scale(glm::vec3(5.0f, 0.1f, 5.0f));
	planePhy->transform->rotate(15.0, glm::vec3(0.0f, 0.0f, 1.0f));
	planePhy->mass = 0.0;
	planePhy->SetRigidbody(phyEng);
	planePhy->name = "planePhy";
	planePhy->rigidBody->setUserPointer(planePhy);

	//delete planePhy;
}

void SceneAudioTest::Update(float deltaTime)
{
	cubePhy->solve(phyEng);
	cube->solve( phyEng );

	if( planePhy != nullptr )
		planePhy->solve(phyEng);

	//camera->RotateViewPoint(800, glfwGetTime());

	if (phyEng->CollisionTest("cubePhy", "planePhy") && !playonce)
	{	cout << "collision" << endl;
		//collisionSound->play2D("resources\\audio\\breakout.mp3", false);
	    playonce = true;   
	}
}

void SceneAudioTest::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if( planePhy != nullptr )
		planePhy->render();

	cubePhy->render();
	cube->render();

	phyEng->dynamicsWorld->debugDrawWorld();
	phyEng->dynamicsWorld->getDebugDrawer()->setDebugMode( btIDebugDraw::DebugDrawModes::DBG_DrawWireframe );
}