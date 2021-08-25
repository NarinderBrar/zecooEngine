#include "Game.h"

Game::Game(Input* _input)
{
	input = _input;

	phyEng = new PhysicsEngine();

	sceneBasic = new SceneBasic(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, phyEng);
	//sceneTriangle = new SceneTriangle(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, phyEng);
	//scene = new Scene(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, phyEng);
}

void Game::Update(float deltaTime)
{
	if (input->getPressedKey() == "Up")
		std::cout << "up key" << std::endl;

	phyEng->Solve(deltaTime);
	
	//sceneTriangle->Update(deltaTime);
	sceneBasic->Update(deltaTime);
	//scene->Update(deltaTime);
}

void Game::Render(float deltaTime)
{
	//sceneTriangle->Render();
	sceneBasic->Render();
	//scene->Render();
}