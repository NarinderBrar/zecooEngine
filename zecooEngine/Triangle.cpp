#include "Triangle.h"

Triangle::Triangle(Material* _material, Texture* _texture): Model(_material, _texture)
{
	triangleMesh = new TriangleMesh();
}

void Triangle::render()
{
	Model::render();
	triangleMesh->Render();
}

Triangle::~Triangle()
{
}
