#pragma once

#include <iostream>

#include "ScenePhyKinematic.h"

#include "Input.h"
#include "Settings.h"

#include "PhysicsEngine.h"

#include "btBulletDynamicsCommon.h"


class Game
{
public:
	Input* input;

	ScenePhyKinematic* scene;
	PhysicsEngine* phyEng;

	Game(Input* _input);

	void Update(float deltaTime);
	void Render(float deltaTime);
};

