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

using namespace glutil;

class Scene
{
public:
	Shader* shader;

	DirectionalLight* dlight;
	Material* material;
	Texture* floorTexture;

	Cube* cube;
	Cube* cubeB;
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

	glm::mat4 I = glm::mat4(1.0);
	MatrixStack matrixStack;
	
	float v = 0.0f;
	float u_time = 0.0f;
	Scene(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine);

	void Update(float deltaTime);

	void Render();
};