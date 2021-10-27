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

#include "UI.h"

class SceneBulletPhysics
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

	Plane* plane;
	Cube* cube;

	Plane* planePhy;
	Cube* cubePhy;

	PhysicsEngine* phyEng;

	Input* input;

	Camera* camera;

	glm::mat4 projection;

	UI* ui;

	SceneBulletPhysics(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input );

	void Update(float deltaTime);

	void Render();

};