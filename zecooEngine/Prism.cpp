#include "Prism.h"

Prism::Prism(Material* _material, Texture* _texture) : Model(_material, _texture)
{
	prismIndexMesh = new PrismIndexMesh();
}

void Prism::render()
{
	Model::render();
	prismIndexMesh->Render();
}

Prism::~Prism()
{
}
