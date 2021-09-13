#include "Tube.h"

Tube::Tube(Material* _material, Texture* _texture) : Model(_material, _texture)
{
	tubeMesh = new TubeMesh();
}

void Tube::render()
{
	Model::render();
	tubeMesh->Render();
}

Tube::~Tube()
{

}