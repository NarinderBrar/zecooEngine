#include "Game.h"

Game::Game(Input* _input)
{
	input = _input;

	phyEng = new PhysicsEngine();

	sceneRobotArm = new SceneRobotArm(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, phyEng, input );
}

void Game::Update(float deltaTime)
{
	phyEng->Solve(deltaTime);
	
	sceneRobotArm->Update(deltaTime);
}

void Game::Render(float deltaTime)
{
	sceneRobotArm->Render();
}