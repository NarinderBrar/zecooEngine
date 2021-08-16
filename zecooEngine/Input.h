#pragma once
#include "Window.h"
#include <string.h>

class Input
{
	GLFWwindow* window;

	public:
		Input(GLFWwindow* _window);
		std::string getPressedKey();
};