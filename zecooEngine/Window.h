#pragma once

#include <windows.h>
#include "Settings.h"

#include <iostream>

#include <glad/glad.h>
#include <glfw3.h>

class Window
{ 
public:
	Window();
	~Window();

	GLFWwindow* getWindow();

	bool isActive();
	void Update();

private:


	GLFWwindow* glfwindow = NULL;
};

