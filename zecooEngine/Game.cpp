#include "Game.h"

Game::Game(Input* _input)
{
	input = _input;

	phyEng = new PhysicsEngine();

	sceneExperiment = new SceneExperiment(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, phyEng, input );
}

void Game::Update(float deltaTime)
{
	phyEng->Solve(deltaTime);
	
	sceneExperiment->Update(deltaTime);
}

void Game::Render(float deltaTime)
{
	sceneExperiment->Render();
}