#include "Game.h"

Game::Game(Input* _input)
{
	input = _input;

	phyEng = new PhysicsEngine();

	//scene = new Scene(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, phyEng);
	sceneA = new SceneA(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, phyEng);
}

void Game::Update(float deltaTime)
{
	if (input->getPressedKey() == "Up")
		std::cout << "up key" << std::endl;

	phyEng->Solve(deltaTime);

	//scene->Update(deltaTime);
	sceneA->Update(deltaTime);
}

void Game::Render(float deltaTime)
{
	//scene->Render();
	sceneA->Render();
}