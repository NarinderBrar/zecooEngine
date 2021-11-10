#include "SceneModelLoad.h"

SceneModelLoad::SceneModelLoad( int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input )
{
	phyEng = physicsEngine;

	// configure global opengl state
	glEnable( GL_DEPTH_TEST );

	camera = new Camera( SCR_WIDTH, SCR_HEIGHT );
	camera->SetPerspectiveProjectionMatrix( glm::radians( 45.0f ), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f );
	glm::vec3 camPos = glm::vec3( 0.0, 300.0, 800.0 );
	glm::vec3 camView = glm::vec3( 0.0, 3.0, 0.0 );
	glm::vec3 camUp = glm::vec3( 0.0, 1.0, 0.0 );
	camera->Set( camPos, camView, camUp );

	texture = new Texture();
	texture->Load( "resources\\textures\\texture_diffuseN.png" );
	texture->SetSamplerObjectParameter( GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	texture->SetSamplerObjectParameter( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	texture->SetSamplerObjectParameter( GL_TEXTURE_WRAP_S, GL_REPEAT );
	texture->SetSamplerObjectParameter( GL_TEXTURE_WRAP_T, GL_REPEAT );

	shader = new Shader( "resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs" );

	dlight = new DirectionalLight( glm::vec3( 0.0, 0.0, 0.0 ), glm::vec3( 0.2, -1.0, 0.2 ) );
	dlight->diffuse = glm::vec3( 1.0, 1.0, 1.0 );
	dlight->ambient = glm::vec3( 0.5, 0.5, 0.5 );

	glm::vec4 color = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
	material = new Material( shader, color );
	material->linkLight( dlight );
	material->linkCamera( camera );

	plane = new Plane( material, NULL );

	cube = new Cube( material, NULL );
	cube->transform->translate( glm::vec3( 0.0f, 0.5f, 5.0f ) );
	cube->transform->rotate( 45.0f, glm::vec3( 0.0f, 1.0f, 0.0f ) );

	modelExternal = new ModelExternal( "resources\\objects\\model\\cube_texture.fbx", NULL );
}

void SceneModelLoad::Update( float deltaTime )
{
	camera->RotateViewPoint( 1800, glfwGetTime() );
}

void SceneModelLoad::Render()
{
	glClearColor( 0.8f, 0.8f, 0.8f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//plane->render();
	//cube->render();
	modelExternal->Draw( material, texture );
}