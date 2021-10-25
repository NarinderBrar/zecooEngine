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
	bool v = false;
	float angle = 0.0;
	float clr[3] = { 0.0,0.0,0.0 };
	UI();
	void Update();
	void Render();
	~UI();
};

