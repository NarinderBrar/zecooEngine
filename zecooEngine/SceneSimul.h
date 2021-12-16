#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Plane.h"
#include "Cube.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"

#include "Input.h"

#include "Platform/Core/EnvironmentVariables.h"
#include "Platform/CrossPlatform/RenderPlatform.h"
#include "Platform/OpenGL/RenderPlatform.h"
#include "Simul/Terrain/BaseTerrainRenderer.h"
#include "Platform/CrossPlatform/Camera.h"
#include "Platform/OpenGL/Texture.h"
#include "Simul/Clouds/BaseWeatherRenderer.h"
#include "Simul/Sky/BaseSkyRenderer.h"
#include "Platform/Core/CommandLineParams.h"
#include "Simul/Shaders/SL/sky_constants.sl"
#include "Platform/Core/Timer.h"
#include "Simul/Clouds/CloudRenderer.h"
#ifdef _MSC_VER
#include "Platform/Windows/VisualStudioDebugOutput.h"
//#define USE_RENDERDOC_DLL
#ifdef USE_RENDERDOC_DLL
#include "Platform/CrossPlatform/RenderDocLoader.h"
#endif
#endif

#ifdef _MSC_VER
static const char* GetErr()
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	return (const char*)lpMsgBuf;
}
#endif

class SceneSimul
{
public:
	Shader* shader;
	DirectionalLight* dlight;
	Material* material;

	Plane* plane;
	Cube* cube;
	Cube* cubeChild;

	PhysicsEngine* phyEng;

	Camera* camera;
	glm::mat4 projection;

	int         mWidth;
	int         mHeight;
	// trueSKY objects
	platform::core::Timer* mAppTimer;
	float                                   mRealTime;
	float									mDayTime;
	float                                   mDeltaTime;
	simul::crossplatform::RenderPlatform* mRenderPlatform;
	simul::opengl::RenderPlatform           mGLPlatform;

	simul::clouds::Environment* mEnvironment;
	simul::clouds::BaseWeatherRenderer* mWeatherRenderer;

	//! Terrain renderer :)
	simul::terrain::BaseTerrainRenderer* mTerrain;

	bool                                    mReversedDepth;
	//! Camera and inut states
	simul::crossplatform::Camera			mCamera;
	simul::crossplatform::MouseCameraState	mMouseCameraState;
	simul::crossplatform::MouseCameraInput	mMouseCameraInput;

	float mCameraSpeed;

	simul::crossplatform::BaseFramebuffer* mHDR_MSAA_Fbo;
	simul::crossplatform::BaseFramebuffer* mFinalFbo;

	simul::crossplatform::Texture* mExternalDepth;
	std::vector<std::string> m_sequencePaths;
	float	m_horizonBlendExp{ 0 };

	SceneSimul( int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input );

	void InitTrueSKY();

	void Update( float deltaTime );

	void Render();

};