#include "SceneTriangle.h"

SceneTriangle::SceneTriangle(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine)
{
	phyEng = physicsEngine;

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	shader = new Shader("resources\\shader\\SceneTriangle.vs", "resources\\shader\\SceneTriangle.fs");

	glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	material = new Material(shader, color);

	triangle = new Triangle(material, NULL);
}

void SceneTriangle::Update(float deltaTime)
{

}

void SceneTriangle::Render()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	triangle->render();
}