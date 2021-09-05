#include "Circle.h"

#include "Circle.h"

Circle::Circle(Material* _material, Texture* _texture) : Model(_material, _texture)
{
	circleMesh = new CircleMesh();
}

void Circle::render()
{
	Model::render();
	circleMesh->Render();
}

Circle::~Circle()
{
}
