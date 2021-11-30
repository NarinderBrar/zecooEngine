#include "SceneBasic.h"

SceneBasic::SceneBasic(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input )
{
	shader = new Shader("resources\\shader\\unlit.vs", "resources\\shader\\unlit.fs");
	
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	
	material = new Material(shader, color);
	triangle = new Triangle(material, NULL);
}

void SceneBasic::Update(float deltaTime)
{
}

void SceneBasic::Render()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	triangle->render();
}
