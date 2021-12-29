#include "Game.h"

Game::Game(Input* _input)
{
	input = _input;

	phyEng = new PhysicsEngine();

	scene = new SceneB(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, phyEng, input );
}

void Game::Update(float deltaTime)
{
	phyEng->Solve(deltaTime);
	
	scene->Update(deltaTime);
}

void Game::Render(float deltaTime)
{
	scene->Render();
}