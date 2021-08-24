#include "Game.h"

Game::Game(Input* _input)
{
	input = _input;

	phyEng = new PhysicsEngine();

	sceneTriangle = new SceneTriangle(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, phyEng);
	//scene = new Scene(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, phyEng);
}

void Game::Update(float deltaTime)
{
	if (input->getPressedKey() == "Up")
		std::cout << "up key" << std::endl;

	phyEng->Solve(deltaTime);
	
	sceneTriangle->Update(deltaTime);
	//scene->Update(deltaTime);
}

void Game::Render(float deltaTime)
{
	sceneTriangle->Render();
	//scene->Render();
}