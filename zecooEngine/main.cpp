#include <iostream>

#include "imgui\imgui.h"
#include "imgui\backends\imgui_impl_glfw.h"
#include "imgui\backends\imgui_impl_opengl3.h"

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

	const char* glsl_version = "#version 130";

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL( glfwindow, true );
	ImGui_ImplOpenGL3_Init( glsl_version );

	//Setup Dear ImGui style
	ImGui::StyleColorsDark;

	while (window->isActive())
	{
		Update();

		window->Update();

		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Render();

		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	window->~Window();
}