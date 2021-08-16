#include "Plane.h"

Plane::Plane(Material* _material, Texture* _texture) : Model(_material, _texture)
{
	planeMesh = new PlaneMesh();
}

void Plane::render()
{
	Model::render();
	planeMesh->Render();
}

Plane::~Plane()
{
}
