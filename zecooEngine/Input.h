#pragma once
#include "Window.h"
#include <string.h>

class Input
{
	GLFWwindow* window;

	public:
		static double mouse_xpos, mouse_ypos;

		Input(GLFWwindow* _window);
		std::string getPressedKey();
};