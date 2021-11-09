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

class ScenePhyConstraint
{
public:
	Texture* floorTexture;
	Texture* cubeTexture;

	Shader* floorShader;
	Shader* cubeShader;

	DirectionalLight* dlight;

	Material* floorMaterial;
	Material* cubeMaterial;

	Plane* planePhy;
	Cube* cubePhy;

	PhysicsEngine* phyEng;

	Input* input;
	Camera* camera;

	ScenePhyConstraint( int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input );

	void Update( float deltaTime );
	void Render();
};


