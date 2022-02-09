#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Grid.h"
#include "Debugger.h"

#include "Plane.h"
#include "Cylinder.h"
#include "Triangle.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"

class Scene
{
public:
	Shader* shader;

	DirectionalLight* dlight;
	Material* material;
	Texture* walls;
	Texture* walls1;
	Texture* floor;
	Texture* windows;
	Texture* tel;
	Texture* tab;
	Texture* marble;
	Texture* sof;
	Texture* pic;

	Plane* plane;
	Cylinder* cylinder;
	Triangle* triangle;

	Cube* cube;
	Cube* cube1;
	Cube* cube2;
	Cube* cube3;
	Cube* tele;
	Cube* table;
	Cube* cube4;
	Cube* cube5;
	Cube* cube6;
	Cube* frame;
	Cube* sofa;
	Cube* sofa2;
	Cube* matrix;

	Grid* grid;
	Debugger* debugger;

	PhysicsEngine* phyEng;

	Camera* camera;
	glm::mat4 projection;

	Scene(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine);

	void Update(float deltaTime);

	void Render();
};