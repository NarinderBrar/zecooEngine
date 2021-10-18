#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Grid.h"
#include "Debugger.h"

#include "Cube.h"
#include "Cylinder.h"
#include "Triangle.h"
#include "Plane.h"
#include "Tube.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"
#include "MatrixStack.h"
#include "Transformations.h"

#include "Input.h"

using namespace glutil;

class SceneRobotArm
{
public:
	Shader* shaderG;
	Shader* shaderB;
	Shader* shaderR;
	Shader* shaderW;

	DirectionalLight* dlight;

	Material* materialG;
	Material* materialB;
	Material* materialR;
	Material* materialW;

	Texture* floorTexture;

	Cube* cubeG;
	Cube* cubeB;
	Cube* cubeR;
	Cube* cubeW;

	Cylinder* cylinder;
	Triangle* triangle;
	Plane* plane;
	Tube* tube;

	Grid* grid;
	Debugger* debugger;

	PhysicsEngine* phyEng;

	Camera* camera;
	glm::mat4 projection;

	glm::mat4 pose;

	double angle = 0;

	glm::mat4 position;
	glm::mat4 rotation;

	glm::mat4 I = glm::mat4( 1.0 );

	glm::vec3 lookAt = glm::vec3( -3, 0, 0 );

	MatrixStack matrixStack;
	Transformations* transformations;

	float cameraMove = 0.0;
	SceneRobotArm( int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input );

	void Update( float deltaTime );

	void Render();
};