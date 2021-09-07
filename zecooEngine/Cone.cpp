#include "Cone.h"
#include "ConeMesh.h"

Cone::Cone(Material* _material, Texture* _texture) : Model(_material, _texture)
{
	coneMesh = new ConeMesh();
}

void Cone::render()
{
	Model::render();

	coneMesh->Render();
}

Cone::~Cone()
{
}
