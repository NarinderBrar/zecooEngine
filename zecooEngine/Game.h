#pragma once

#include <iostream>

#include "SceneB.h"

#include "Input.h"
#include "Settings.h"

#include "PhysicsEngine.h"

#include "btBulletDynamicsCommon.h"


class Game
{
public:
	Input* input;

	SceneB* scene;
	PhysicsEngine* phyEng;

	Game(Input* _input);

	void Update(float deltaTime);
	void Render(float deltaTime);
};

