#include "Input.h"

Input::Input(GLFWwindow* _window)
{
	window = _window;
}

std::string Input::getPressedKey()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return "Esc";
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		return "Up";

	return "";
}

