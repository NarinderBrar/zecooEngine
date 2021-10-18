#pragma once
#include "imgui\imgui.h"
#include "imgui\backends\imgui_impl_glfw.h"
#include "imgui\backends\imgui_impl_opengl3.h"

class UI
{
public:
	float rotation = 0.0;
	float translation = 0.0;
	float color[3] = { 1.0f,1.0f,1.0f };

	UI();
	void Update();
	void Render();
	~UI();
};

