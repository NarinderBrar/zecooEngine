#pragma once

#include "SceneA.h"

#include "SceneExperiment.h"

#include <iostream>

#include "Input.h"
#include "Settings.h"

#include "PhysicsEngine.h"

#include "btBulletDynamicsCommon.h"


class Game
{
public:
	Input* input;

	//SceneA* sceneA;

	SceneExperiment* sceneExperiment;

	PhysicsEngine* phyEng;

	Game(Input* _input);

	void Update(float deltaTime);
	void Render(float deltaTime);
};

