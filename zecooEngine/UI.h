#pragma once
#include "imgui\imgui.h"
#include "imgui\backends\imgui_impl_glfw.h"
#include "imgui\backends\imgui_impl_opengl3.h"

#include<iostream>
#include <string.h>
using namespace std;

class UI
{
public:
	bool show_app_main_menu_bar = false;
	bool show_app_console = false;
	bool show_app_log = false;
	bool show_app_layout = false;

	float f = 0.0;
	bool b = false;
	int k = 0;

	UI();

	void ShowUserGuide();
	void Update();
	void Render();
	~UI();
};

