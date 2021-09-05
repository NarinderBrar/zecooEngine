#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "Transform.h";

#include "CylinderMesh.h";

class Cylinder :public Model
{
private:
	CylinderMesh* cylinderMesh;

public:
	Cylinder(Material* material, Texture* texture);

	void render();

	~Cylinder();
};

