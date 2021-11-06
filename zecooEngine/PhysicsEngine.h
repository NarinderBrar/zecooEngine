#pragma once

#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>

#include "Model.h"

#include <btBulletDynamicsCommon.h>
#include <iostream>

using namespace std;

class PhysicsEngine
{
private:
	
	btDefaultCollisionConfiguration* collisionConfiguration;

	btSequentialImpulseConstraintSolver* solver;
	btBroadphaseInterface* overlappingPairCache;
	btCollisionDispatcher* dispatcher;

public:
	std::map<const btCollisionObject*, std::vector<btManifoldPoint*>> objectsCollisions;

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	btDiscreteDynamicsWorld* dynamicsWorld;

	PhysicsEngine();

	void Solve(float deltaTime);
	bool CollisionTest( std::string bodyA, std::string bodyB );
    //void myTickCallback( btDynamicsWorld* dynamicsWorld, btScalar timeStep );
	void Destroy();

};

