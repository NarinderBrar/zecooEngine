#include "Triangle.h"

Triangle::Triangle(Material* _material, Texture* _texture): Model(_material, _texture)
{
	triangleIndexMesh = new TriangleIndexMesh();
}

void Triangle::render()
{
	Model::render();
	triangleIndexMesh->Render();
}

Triangle::~Triangle()
{
}
