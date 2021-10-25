#include "Game.h"

Game::Game(Input* _input)
{
	input = _input;

	phyEng = new PhysicsEngine();

	sceneUI = new SceneUI(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, phyEng, input );
}

void Game::Update(float deltaTime)
{
	phyEng->Solve(deltaTime);
	
	sceneUI->Update(deltaTime);
}

void Game::Render(float deltaTime)
{
	sceneUI->Render();
}