#include "CustomModel.h"

CustomModel::CustomModel(Material* _material, Texture* _texture) : Model(_material, _texture)
{
	customMesh = new CustomMesh();
}

void CustomModel::render()
{
	Model::render();
	customMesh->Render();
}

CustomModel::~CustomModel()
{
}
