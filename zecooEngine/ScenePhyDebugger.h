#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Plane.h"
#include "Cube.h"
#include "CustomModel.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"
#include "GLDebugDrawer.h"

#include "Input.h"

#include "UI.h"

class ScenePhyDebugger
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
	CustomModel* customModel;

	PhysicsEngine* phyEng;
	GLDebugDrawer* debugDraw;

	Input* input;

	Camera* camera;

	ScenePhyDebugger( int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input );


	void Update( float deltaTime );

	void Render();

};


