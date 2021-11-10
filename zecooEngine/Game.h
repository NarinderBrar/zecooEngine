#pragma once

#include <iostream>

#include "ScenePhyRayTest.h"

#include "Input.h"
#include "Settings.h"

#include "PhysicsEngine.h"

#include "btBulletDynamicsCommon.h"


class Game
{
public:
	Input* input;

	ScenePhyRayTest* scene;
	PhysicsEngine* phyEng;

	Game(Input* _input);

	void Update(float deltaTime);
	void Render(float deltaTime);
};

