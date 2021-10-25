#include "Model.h"

Model::Model(Material* _material, Texture* _texture)
{
	material = _material;
	texture = _texture;
	transform = new Transform();
}

void Model::render()
{
	material->shader->use();
	material->shader->setMat4("model", transform->pose);
	material->update();

	if (texture != NULL)
	{
		texture->Bind();
		material->shader->setInt("textureSample", 1);
	}
	else
	{
		material->shader->setInt("textureSample", 0);
	}
}

Model::~Model()
{

}
