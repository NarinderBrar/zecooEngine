#pragma once
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

class SceneRollingBall
{
public:
	Texture* floorTexture;
	Texture* cubeTexture;

	Shader* floorShader;
	Shader* cubeShader;

	DirectionalLight* dlight;

	SpotLight* slight;

	Material* floorMaterial;
	Material* cubeMaterial;

	Cube* cube;

	Plane* planePhy;
	Cube* cubePhy;
	Sphere* sphere;

	PhysicsEngine* phyEng;
	GLDebugDrawer* debugDraw;

	Input* input;

	Camera* camera;

	SceneRollingBall( int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input );
	void Update( float deltaTime );

	void Render();

};