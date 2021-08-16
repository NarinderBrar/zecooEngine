#include <iostream>

#include "Window.h"
#include "Input.h"
#include "Game.h"
#include "Settings.h"

Input* input;
Game* game;

GLFWwindow* glfwindow;

//Time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void Update()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	game->Update(deltaTime);

	if (input->getPressedKey() == "Esc")
		glfwSetWindowShouldClose(glfwindow, true);
}

void Render()
{
	game->Render(deltaTime);
}

void loadGladFP()
{
	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "Failed to initialize GLAD" << std::endl;
}

int main()
{
	Window* window = new Window();
	glfwindow = window->getWindow();
	
	loadGladFP();

	input = new Input(glfwindow);
	game = new Game(input);

	while (window->isActive())
	{
		Update();
		Render();

		window->Update();
	}

	window->~Window();
}