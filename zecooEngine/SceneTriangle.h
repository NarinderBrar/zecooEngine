#pragma once
#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Plane.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"

class SceneTriangle
{
public:
	Shader* shader;

	DirectionalLight* dlight = NULL;
	Material* material;

	Plane* plane;

	PhysicsEngine* phyEng;

	SceneTriangle(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine);

	void Update(float deltaTime);
	void Render();
};