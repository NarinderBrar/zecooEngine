#include "SceneBasic.h"

SceneBasic::SceneBasic(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine)
{

}

void SceneBasic::Update(float deltaTime)
{
}

void SceneBasic::Render()
{
	glClearColor(0.0f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
