#include "SceneRobotArm.h"

SceneRobotArm::SceneRobotArm( int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input )
{
	phyEng = physicsEngine;

	// configure global opengl state
	glEnable( GL_DEPTH_TEST );

	floorTexture = new Texture();
	floorTexture->Load( "resources\\textures\\cylinder.jpg" );
	floorTexture->SetSamplerObjectParameter( GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	floorTexture->SetSamplerObjectParameter( GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	floorTexture->SetSamplerObjectParameter( GL_TEXTURE_WRAP_S, GL_REPEAT );
	floorTexture->SetSamplerObjectParameter( GL_TEXTURE_WRAP_T, GL_REPEAT );

	camera = new Camera( SCR_WIDTH, SCR_HEIGHT );
	camera->SetPerspectiveProjectionMatrix( glm::radians( 45.0f ), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f );
	glm::vec3 camPos = glm::vec3( 300.0, 300.0, 500.0 );
	glm::vec3 camView = glm::vec3( 0.0, 2.0, 0.0 );
	glm::vec3 camUp = glm::vec3( 0.0, 1.0, 0.0 );
	camera->Set( camPos, camView, camUp );

	shaderG = new Shader( "resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs" );
	shaderB = new Shader( "resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs" );
	shaderR = new Shader( "resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs" );
	shaderW = new Shader( "resources\\shader\\3.3.shader.vs", "resources\\shader\\3.3.shader.fs" );

	grid = new Grid( camera );

	dlight = new DirectionalLight( glm::vec3( 0.0, 0.0, 0.0 ), glm::vec3( 0.2, -1.0, 0.2 ) );
	dlight->diffuse = glm::vec3( 1.0, 1.0, 1.0 );
	dlight->ambient = glm::vec3( 0.5, 0.5, 0.5 );

	glm::vec4 colorG = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
	materialG = new Material( shaderG, colorG );
	materialG->linkLight( dlight );
	materialG->linkCamera( camera );

	glm::vec4 colorB = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
	materialB = new Material( shaderB, colorB );
	materialB->linkLight( dlight );
	materialB->linkCamera( camera );

	glm::vec4 colorR = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
	materialR = new Material( shaderR, colorR );
	materialR->linkLight( dlight );
	materialR->linkCamera( camera );

	glm::vec4 colorW = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
	materialW = new Material( shaderW, colorW );
	materialW->linkLight( dlight );
	materialW->linkCamera( camera );

	debugger = new Debugger( camera );

	std::string versionString = std::string( (const char*)glGetString( GL_VERSION ) );
	debugger->printMsg( "OpenGl : " + versionString );

	cubeR = new Cube( materialR, NULL );
	cubeG = new Cube( materialG, NULL );
	cubeB = new Cube( materialB, NULL );
	cubeW = new Cube( materialW, NULL );

	//cubeR
	//cubeR->transform->rotate( glm::radians( 45.0 ), glm::vec3( 0, 1, 0 ) );

	//cubeG
	cubeG->transform->parent = cubeR->transform;
	cubeG->transform->rotate( glm::radians( -45.0 ), glm::vec3( 1, 0, 0 ) );
	cubeG->transform->translate( glm::vec3( 0, 0.2, 0 ) );

	//cubeB
	cubeB->transform->parent = cubeG->transform;
	cubeB->transform->translate( glm::vec3( 0, 0.5, 0 ) );

	//cubeW
	cubeW->transform->parent = cubeB->transform;
	cubeW->transform->translate( glm::vec3( 0.2, 0.6, 0 ) );

	//glm::vec4 clr = glm::vec4(0,1,0,1);
	//glm::vec3 forward = glm::vec3(cubeR->transform->pose[2][0], cubeR->transform->pose[2][1], cubeR->transform->pose[2][2] );
	//debugger->addRay(cubeR->transform->getPosition(), -forward, clr);

	emptyObject = new EmptyObject();

	emptyObject->transform->position(glm::vec3(0.0,0.0,0.0));
}

void SceneRobotArm::Update( float deltaTime )
{
	cubeR->transform->Update();
	//cubeR->transform->rotate( glm::radians( 50 * deltaTime ), glm::vec3( 0, 1, 0 ) );
	//cubeR->transform->translate( glm::vec3( 0, 0, deltaTime ) );

	I = glm::mat4( 1.0 );
	I = glm::scale( I, glm::vec3( 1.0, 0.2, 1.0 ) );
	cubeR->transform->pose = cubeR->transform->pose * I;

	//cubeG
	cubeG->transform->resetParentScale( glm::vec3( 1, 0.2, 1 ) );
	cubeG->transform->Update();

	angle += deltaTime * 1.0;
	cubeG->transform->rotate( glm::radians( glm::sin( angle )* deltaTime ) * 60, glm::vec3( 1, 0, 0 ) );

	I = glm::mat4( 1.0 );
	I = glm::translate( I, glm::vec3( 0, 0.5, 0 ) );
	cubeG->transform->pose = cubeG->transform->pose * I;

	I = glm::mat4( 1.0 );
	I = glm::scale( I, glm::vec3( 0.2, 1, 0.2 ) );
	cubeG->transform->pose = cubeG->transform->pose * I;

	//cubeB
	cubeB->transform->resetParentScale( glm::vec3( 0.2, 1, 0.2 ) );
	cubeB->transform->Update();

	cubeB->transform->rotate( glm::radians( deltaTime * 300 ), glm::vec3( 0, 1, 0 ) );

	I = glm::mat4( 1.0 );
	I = glm::translate( I, glm::vec3( 0, 0.5, 0 ) );
	cubeB->transform->pose = cubeB->transform->pose * I;

	I = glm::mat4( 1.0 );
	I = glm::scale( I, glm::vec3( 0.2, 1, 0.2 ) );
	cubeB->transform->pose = cubeB->transform->pose * I;

	//cubeW
	cubeW->transform->resetParentScale( glm::vec3( 0.2, 1, 0.2 ) );
	cubeW->transform->Update();

	I = glm::mat4( 1.0 );
	I = glm::scale( I, glm::vec3( 0.6, 0.2, 0.3 ) );
	cubeW->transform->pose = cubeW->transform->pose * I;

	//camera->RotateViewPoint( 500, glfwGetTime() );
}

void SceneRobotArm::Render()
{
	glClearColor( 0.8f, 0.8f, 0.8f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	debugger->draw();

	grid->Render();

	cubeR->render();
	cubeG->render();
	cubeB->render();
	cubeW->render();
}