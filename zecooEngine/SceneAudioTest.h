#pragma once
#include <irrKlang.h>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Plane.h"
#include "Cube.h"
#include "Sphere.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"
#include "GLDebugDrawer.h"

#include "Input.h"

#include "UI.h"

using namespace irrklang;
using namespace std;

class SceneAudioTest
{
public:
	Texture* floorTexture;
	Texture* cubeTexture;

	Shader* floorShader;
	Shader* cubeShader;

	DirectionalLight* dlight;

	PointLight* plight0;
	PointLight* plight1;
	PointLight* plight2;
	PointLight* plight3;

	SpotLight* slight;

	Material* floorMaterial;
	Material* cubeMaterial;

	Cube* cube;

	Plane* planePhy;
	Cube* cubePhy;
	Sphere* customModel;

	PhysicsEngine* phyEng;
	GLDebugDrawer* debugDraw;

	ISoundEngine* collisionSound = createIrrKlangDevice();

	Input* input;

	Camera* camera;

	glm::mat4 projection;

	float k = 0.0;
	bool playonce = false;

	SceneAudioTest(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input);


	void Update(float deltaTime);

	void Render();

};

