#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "ModelExternal.h"

#include "Plane.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"

#include "Input.h"
#include "Cube.h"

class SceneModelLoad
{
public:
	Shader* shader;

	DirectionalLight* dlight = NULL;
	Material* material;
	Texture* texture;

	Cube* cube;

	ModelExternal* modelExternal;
	Plane* plane;
	Camera* camera;
	PhysicsEngine* phyEng;

	SceneModelLoad( int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input );

	void Update( float deltaTime );
	void Render();
};