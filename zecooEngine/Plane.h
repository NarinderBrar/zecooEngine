#pragma once

#include "Model.h";
#include "Material.h";
#include "Texture.h";
#include "PlaneMesh.h";
#include "Transform.h";

class Plane:public Model
{
	private:
		PlaneMesh* planeMesh;

	public:
		Plane(Material* _material, Texture* _texture);

		void render();

		~Plane();
};

