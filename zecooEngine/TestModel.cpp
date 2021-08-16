#include "TestModel.h"

TestModel::TestModel(Material* _material, Texture* _texture) : Model(_material, _texture)
{
	testMesh = new TestMesh();
}

void TestModel::render()
{
	Model::render();
	testMesh->Render();
}

TestModel::~TestModel()
{
}
