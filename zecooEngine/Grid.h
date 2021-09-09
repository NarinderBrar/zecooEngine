#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "Cube.h"
#include "Plane.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "PhysicsEngine.h"

class Grid
{
public:
	Shader* shaderX;
	Shader* shaderZ;
	Shader* shaderGrid;

	Material* materialX;
	Material* materialZ;
	Material* materialGrid;

	Plane* planes[22];

	Grid(Camera* camera);

	void Render();
};