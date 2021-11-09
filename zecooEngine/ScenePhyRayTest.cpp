#include "ScenePhyRayTest.h"

ScenePhyRayTest::ScenePhyRayTest( int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input )
{
	// configure global opengl state
	glEnable( GL_DEPTH_TEST );

	phyEng = physicsEngine;

	input = _input;

	floorTexture = new Texture();
	floorTexture->Load( "resources\\textures\\brickwall.jpg" );
	floorTexture->SetSamplerObjectParameter( GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	floorTexture->SetSamplerObjectParameter( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	floorTexture->SetSamplerObjectParameter( GL_TEXTURE_WRAP_S, GL_REPEAT );
	floorTexture->SetSamplerObjectParameter( GL_TEXTURE_WRAP_T, GL_REPEAT );

	cubeTexture = new Texture();
	cubeTexture->Load( "resources\\textures\\container.jpg" );
	cubeTexture->SetSamplerObjectParameter( GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	cubeTexture->SetSamplerObjectParameter( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	cubeTexture->SetSamplerObjectParameter( GL_TEXTURE_WRAP_S, GL_REPEAT );
	cubeTexture->SetSamplerObjectParameter( GL_TEXTURE_WRAP_T, GL_REPEAT );

	camera = new Camera( SCR_WIDTH, SCR_HEIGHT );
	camera->SetPerspectiveProjectionMatrix( glm::radians( 45.0f ), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f );
	glm::vec3 camPos = glm::vec3( 0.0, 300.0, 800.0 );
	glm::vec3 camView = glm::vec3( 0.0, 3.0, 0.0 );
	glm::vec3 camUp = glm::vec3( 0.0, 1.0, 0.0 );
	camera->Set( camPos, camView, camUp );

	cubeShader = new Shader( "resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs" );

	dlight = new DirectionalLight( glm::vec3( 0.0, 0.0, 0.0 ), glm::vec3( 0.2, -1.0, 0.2 ) );
	dlight->diffuse = glm::vec3( 1.0, 1.0, 1.0 );
	dlight->ambient = glm::vec3( 0.5, 0.5, 0.5 );

	glm::vec4 color = glm::vec4( 0.5f, 0.5f, 0.5f, 0.5f );
	floorMaterial = new Material( cubeShader, color );
	floorMaterial->linkLight( dlight );
	floorMaterial->linkCamera( camera );

	cubeMaterial = new Material( cubeShader, color );
	cubeMaterial->linkLight( dlight );
	cubeMaterial->linkCamera( camera );

	cubePhy = new Cube( cubeMaterial, cubeTexture );
	cubePhy->transform->translate( glm::vec3( 0.0f, 6.0f, 0.0f ) );
	cubePhy->mass = 1.0;
	cubePhy->SetRigidbody( phyEng );

	planePhy = new Plane( floorMaterial, floorTexture );
	planePhy->transform->scale( glm::vec3( 1.0f, 1.0f, 1.0f ) );
	planePhy->transform->rotate( 0.0, glm::vec3( 0.0f, 0.0f, 1.0f ) );
	planePhy->mass = 0.0;
	planePhy->SetRigidbody( phyEng );
}

void ScenePhyRayTest::RayCast()
{
	btVector3 from( 0, 0, 0 );
	btVector3 to( 0, 11, 0 );
	//phyEng->dynamicsWorld->getDebugDrawer()->drawLine( from, to, btVector4( 0, 0, 1, 1 ) );

	btCollisionWorld::ClosestRayResultCallback closestResults( from, to );
	closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

	phyEng->dynamicsWorld->rayTest( from, to, closestResults );

	if( closestResults.hasHit() )
	{
		btVector3 p = from.lerp( to, closestResults.m_closestHitFraction );
		//phyEng->dynamicsWorld->getDebugDrawer()->drawSphere( p, 0.1, blue );
		//m_dynamicsWorld->getDebugDrawer()->drawLine( p, p + closestResults.m_hitNormalWorld, blue );
	}
}

void ScenePhyRayTest::Update( float deltaTime )
{
	if( input->getPressedKey() == "Up" )
		std::cout << "up key" << std::endl;

	cubePhy->solve( phyEng );
	planePhy->solve( phyEng );

	camera->RotateViewPoint( 800, glfwGetTime() );
}

void ScenePhyRayTest::Render()
{
	glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	planePhy->render();
	cubePhy->render();
}