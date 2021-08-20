#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Plane.h"
#include "Triangle.h"
#include "Cube.h"
#include "TestModel.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"

class SceneC
{
public:
	Texture* texture;
	Texture* texture1;

	Shader* shader;
	Shader* shaderL;
	Shader* shaderT;
	Shader* testShader;

	DirectionalLight* dlight;

	PointLight* plight0;
	PointLight* plight1;
	PointLight* plight2;
	PointLight* plight3;

	SpotLight* slight;

	Material* material;
	Material* materialLight;
	Material* testMaterial;

	Plane* plane;
	Cube* cube;
	Cube* cubeLight;
	Triangle* triangle1;
	TestModel* testModel;

	PhysicsEngine* phyEng;

	Camera* camera;

	// positions of the point lights
	glm::vec3 pointLightPositions[6] = {
		glm::vec3(0.0f,  1.5f,  0.0f),
		glm::vec3(0.5f,  0.5f,  0.0f),
		glm::vec3(0.0f,  0.5f,  0.5f),
		glm::vec3(0.0f,  0.5f,  0.75f),
		glm::vec3(1.0f,  0.5f,  0.75f),
		glm::vec3(1.5f,  0.5f,  0.75f),
	};

	glm::mat4 projection;

	SceneC(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine);

	void Update(float deltaTime);

	void Render();

};