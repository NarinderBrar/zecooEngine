#pragma once

#include "SceneTriangle.h"
#include "Scene.h"
#include "SceneBasic.h"

#include <iostream>

#include "Input.h"
#include "Settings.h"

#include "PhysicsEngine.h"

#include "btBulletDynamicsCommon.h"

class Game
{
public:
	Input* input;

	//Scene* scene;
	//SceneBasic* sceneBasic;
	SceneTriangle* sceneTriangle;

	PhysicsEngine* phyEng;

	Game(Input* _input);

	void Update(float deltaTime);
	void Render(float deltaTime);
};

