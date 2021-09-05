#pragma once

#include "Scene.h"

#include <iostream>

#include "Input.h"
#include "Settings.h"

#include "PhysicsEngine.h"

#include "btBulletDynamicsCommon.h"

class Game
{
public:
	Input* input;

	Scene* scene;

	PhysicsEngine* phyEng;

	Game(Input* _input);

	void Update(float deltaTime);
	void Render(float deltaTime);
};

