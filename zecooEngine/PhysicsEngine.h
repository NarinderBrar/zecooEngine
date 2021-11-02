#pragma once

#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>

#include <btBulletDynamicsCommon.h>


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
    //void myTickCallback( btDynamicsWorld* dynamicsWorld, btScalar timeStep );
	void Destroy();

};

