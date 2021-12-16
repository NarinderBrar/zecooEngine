#include "SceneSimul.h"

SceneSimul::SceneSimul( int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input )
{
	mWidth = SCR_WIDTH;
	mHeight = SCR_HEIGHT;

	phyEng = physicsEngine;

	// configure global opengl state
	glEnable( GL_DEPTH_TEST );

	camera = new Camera( SCR_WIDTH, SCR_HEIGHT );
	camera->SetPerspectiveProjectionMatrix( glm::radians( 45.0f ), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f );
	glm::vec3 camPos = glm::vec3( 0.0, 300.0, 800.0 );
	glm::vec3 camView = glm::vec3( 0.0, 3.0, 0.0 );
	glm::vec3 camUp = glm::vec3( 0.0, 1.0, 0.0 );
	camera->Set( camPos, camView, camUp );

	shader = new Shader( "resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs" );

	dlight = new DirectionalLight( glm::vec3( 0.0, 0.0, 0.0 ), glm::vec3( 0.2, -1.0, 0.2 ) );
	dlight->diffuse = glm::vec3( 1.0, 1.0, 1.0 );
	dlight->ambient = glm::vec3( 0.5, 0.5, 0.5 );

	glm::vec4 color = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
	material = new Material( shader, color );
	material->linkLight( dlight );
	material->linkCamera( camera );

	plane = new Plane( material, NULL );
	plane->transform->scale( glm::vec3( 5.0f, 5.0f, 5.0f ) );

	cube = new Cube( material, NULL );
	cube->transform->position( glm::vec3( 0, 2, 0 ) );
	cube->transform->translate( glm::vec3( 0.0f, 0.0f, 2.0f ) );
	glm::vec3 a = glm::normalize( glm::vec3( 5, 0, 0 ) );
	glm::vec3 b = glm::vec3( -1, 0, 0 );
	float c = glm::dot( a, b );

	mRenderPlatform=  &mGLPlatform ;
	mEnvironment = nullptr;

	InitTrueSKY();
}

void SceneSimul::InitTrueSKY()
{
	mRenderPlatform->PushShaderBinaryPath( "/Vendor/Simul/build//Platform/OpenGL/shaderbin" );
	mRenderPlatform->PushShaderPath( "/Vendor/Simul/Platform/Shaders/SFX/" );
	mRenderPlatform->PushShaderPath( "/Vendor/Simul/Platform/Shaders/SL/" );
	mRenderPlatform->PushTexturePath( "/Vendor/Simul/Media/Textures" );
	std::string simul_env = simul::base::EnvironmentVariables::GetSimulEnvironmentVariable( "SIMUL" );
	if( simul_env.length() )
	{
		std::string texturesPath = simul_env + "/Media/textures";
		std::cout << "Set Textures path: " << texturesPath << std::endl;
		mRenderPlatform->PushTexturePath( texturesPath.c_str() );
		mRenderPlatform->PushTexturePath( ( simul_env + "/Media/textures" ).c_str() );

		// Shader binaries: we want to use a shared common directory under Simul/Media. But if we're running from some other place, we'll just create a "shaderbin" directory.

		mRenderPlatform->PushShaderBinaryPath( ( simul_env + "/Platform/OpenGL/shaderbin" ).c_str() );
		mRenderPlatform->PushShaderBinaryPath( ( simul_env + "/build/Platform/OpenGL/shaderbin" ).c_str() );
		mRenderPlatform->PushTexturePath( ( simul_env + "/Media/Textures" ).c_str() );

		mRenderPlatform->PushShaderBinaryPath( "shaderbin" );
	}

	mRenderPlatform->RestoreDeviceObjects( nullptr );

	// Init env:
	mEnvironment = new simul::clouds::Environment();
	{
		simul::clouds::CloudKeyframer* ck1 = mEnvironment->CreateCloudKeyframer( nullptr, false, 1 );
		simul::clouds::CloudKeyframer* ck2 = mEnvironment->CreateCloudKeyframer( nullptr, false, 2 );

		ck1->AddStorm( 0.f, 1.f, simul::crossplatform::Quaterniond(), 100.f );
		simul::clouds::CloudKeyframe* k1 = ck1->InsertKeyframe( 0.5f );
		k1->cloudiness = 0.55f;
		k1->cloud_base_km = 1.0f;
		k1->cloud_height_km = 4.0f;

		static float rainOrSnow = 0.0f;
		static float strength = 1.0f;
		static float streaksStrength = 0.0f;
		static bool lockToClouds = false;
		static bool regional = false;
		static float rainRadiusKm = 100.0f;

		k1->rain_to_snow = rainOrSnow;
		k1->precipitation = strength;
		k1->precipitationRegion.rainEffectStrength = streaksStrength;
		k1->precipitationRegion.lockToClouds = lockToClouds;
		k1->precipitationRegion.regional = regional;
		k1->precipitationRegion.radiusKm = rainRadiusKm;

		simul::clouds::CloudKeyframe* k2 = ck2->InsertKeyframe( 0.5 );
		k2->cloudiness = 0.65f;
		k2->cloud_base_km = 8.0f;
		k1->cloud_height_km = 2.0f;
	}

	// Init weather renderer:
	mWeatherRenderer = new simul::clouds::BaseWeatherRenderer( mEnvironment, nullptr );
	mWeatherRenderer->RestoreDeviceObjects( mRenderPlatform );

	auto& cloudOptions = mWeatherRenderer->GetCloudRenderer()->GetCloudRenderingOptions();
	cloudOptions.MaximumCubemapResolution = 512;
	cloudOptions.RaytraceMode = simul::clouds::STANDARD_RENDERING;
	cloudOptions.WindSpeedMS = vec3( 0.01f, 0.f, 0.f );
	cloudOptions.DefaultAmortization = 0;

	// Init terrain renderer
	mTerrain = new simul::terrain::BaseTerrainRenderer( nullptr );
	mTerrain->RestoreDeviceObjects( mRenderPlatform );
	mTerrain->SetBaseSkyInterface( mEnvironment->skyKeyframer );

	// Config camera
	mCamera.SetPositionAsXYZ( 0.0f, 0.0f, 500.0f );
	float look[] = { 0.f,1.f,0.f }, up[] = { 0.f,0.f,1.f };
	mCamera.LookInDirection( look, up );
	mCamera.SetHorizontalFieldOfViewDegrees( 85.f );

	simul::crossplatform::CameraViewStruct cvs;
	cvs = mCamera.GetCameraViewStruct();
	cvs.exposure = 0.5f;
	mCamera.SetCameraViewStruct( cvs );

	mAppTimer = new platform::core::Timer();

	// auto e = mRenderPlatform->CreateEffect("test");
	// e->Apply(simul::crossplatform::DeviceContext(), "testtech", "p0");

	mHDR_MSAA_Fbo = mRenderPlatform->CreateFramebuffer( "hdrfbo" );
	mHDR_MSAA_Fbo->SetWidthAndHeight( mWidth, mHeight );
	mHDR_MSAA_Fbo->SetAntialiasing( 8 );
	mHDR_MSAA_Fbo->SetFormat( simul::crossplatform::RGBA_16_FLOAT );
	mHDR_MSAA_Fbo->SetDepthFormat( simul::crossplatform::D_32_FLOAT );
	mHDR_MSAA_Fbo->RestoreDeviceObjects( mRenderPlatform );

	mFinalFbo = mRenderPlatform->CreateFramebuffer( "finalfbo" );
	mFinalFbo->SetWidthAndHeight( mWidth, mHeight );
	// It cannot be bigger than ONE
	mFinalFbo->SetAntialiasing( 1 );
	mFinalFbo->SetFormat( simul::crossplatform::RGBA_16_FLOAT );
	mFinalFbo->RestoreDeviceObjects( mRenderPlatform );

	mExternalDepth = mRenderPlatform->CreateTexture( "externaldepth" );
}

void SceneSimul::Update( float deltaTime )
{
	camera->Update( deltaTime );
}

void SceneSimul::Render()
{
	glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	plane->render();
	cube->render();
}