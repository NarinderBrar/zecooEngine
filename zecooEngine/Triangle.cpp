#include "Triangle.h"

Triangle::Triangle(Material* _material, Texture* _texture): Model(_material, _texture)
{
	triangleMesh = new TriangleMesh();
	//triangleIndexMesh = new TriangleIndexMesh();
}

void Triangle::render()
{
	Model::render();
	triangleMesh->Render();
	//triangleIndexMesh->Render();
}

Triangle::~Triangle()
{
}
