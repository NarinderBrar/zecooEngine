#include "Grid.h"

Grid::Grid(Camera* camera)
{
	shaderX = new Shader("resources\\shader\\unlit.vs", "resources\\shader\\unlit.fs");
	shaderZ = new Shader("resources\\shader\\unlit.vs", "resources\\shader\\unlit.fs");
	shaderGrid = new Shader("resources\\shader\\unlit.vs", "resources\\shader\\unlit.fs");

	float offsetY = -0.1;
	glm::vec4 colorX = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	materialX = new Material(shaderX, colorX);
	materialX->linkCamera(camera);

	glm::vec4 colorZ = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	materialZ = new Material(shaderZ, colorZ);
	materialZ->linkCamera(camera);

	glm::vec4 color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	materialGrid = new Material(shaderGrid, color);
	materialGrid->linkCamera(camera);

	int k = 0;
	for (int i = 0; i < 10; i+=2)
	{
		Plane* grid = new Plane(materialGrid, NULL);
		grid->transform->position(glm::vec3(k+1, offsetY, 0.0f));
		grid->transform->scale(glm::vec3(0.01, offsetY, 10.0f));
		planes[i] = grid;

		grid = new Plane(materialGrid, NULL);
		grid->transform->position(glm::vec3(-1 * (k+1), offsetY, 0.0f));
		grid->transform->scale(glm::vec3(0.01, offsetY, 10.0f));
		planes[i+1] = grid;

		k++;
	}

	k = 0;
	for (int i = 10; i < 20; i += 2)
	{
		Plane* grid = new Plane(materialGrid, NULL);
		grid->transform->position(glm::vec3(0.0f, offsetY, k + 1));
		grid->transform->scale(glm::vec3(10.0, offsetY, 0.01f));
		planes[i] = grid;

		grid = new Plane(materialGrid, NULL);
		grid->transform->position(glm::vec3(0.0f, offsetY, -1 * (k + 1)));
		grid->transform->scale(glm::vec3(10.0, offsetY, 0.01f));
		planes[i+1] = grid;

		k++;
	}

	Plane* x_axis = new Plane(materialX, NULL);
	x_axis->transform->position(glm::vec3(0.0f, offsetY, 0.0f));
	x_axis->transform->scale(glm::vec3(10, 0.05f, 0.05f));
	planes[20] = x_axis;

	Plane* y_axis = new Plane(materialZ, NULL);
	y_axis->transform->position(glm::vec3(0.0f, offsetY, 0.0f));
	y_axis->transform->scale(glm::vec3(0.05, 0.05f, 10.0f));
	planes[21] = y_axis; 
}

void Grid::Render()
{
	for (size_t i = 0; i < 22; i++)
	{
		planes[i]->render();
	}
}