#include "Pyramid.h"

Pyramid::Pyramid(Material* _material, Texture* _texture) : Model(_material, _texture)
{
	//triangleMesh = new TriangleMesh();
	pyramidMesh = new PyramidMesh();
}

void Pyramid::render()
{
	Model::render();
	//triangleMesh->Render();
	pyramidMesh->Render();
}

Pyramid::~Pyramid()
{
}
