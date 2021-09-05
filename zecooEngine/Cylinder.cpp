#include "Cylinder.h"

Cylinder::Cylinder(Material* _material, Texture* _texture) : Model(_material, _texture)
{
	cylinderMesh = new CylinderMesh();
}

void Cylinder::render()
{
	Model::render();
	cylinderMesh->Render();
}

Cylinder::~Cylinder()
{

}
