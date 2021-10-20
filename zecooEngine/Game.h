#pragma once

#include "SceneRobotArm.h"

#include <iostream>

#include "Input.h"
#include "Settings.h"

#include "PhysicsEngine.h"

#include "btBulletDynamicsCommon.h"


class Game
{
public:
	Input* input;

	SceneRobotArm* sceneRobotArm;
	PhysicsEngine* phyEng;

	Game(Input* _input);

	void Update(float deltaTime);
	void Render(float deltaTime);
};

