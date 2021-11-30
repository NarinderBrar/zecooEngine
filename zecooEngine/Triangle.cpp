#include "Triangle.h"

Triangle::Triangle(Material* _material, Texture* _texture): Model(_material, _texture)
{
	triangleIndexMesh = new TriangleMesh();
}

void Triangle::render()
{
	Model::render();
	triangleIndexMesh->Render();
}

Triangle::~Triangle()
{
}
