#pragma once
#include "btBulletDynamicsCommon.h"

class PhysicsEngine
{
private:
	btDefaultCollisionConfiguration* collisionConfiguration;

	btSequentialImpulseConstraintSolver* solver;
	btBroadphaseInterface* overlappingPairCache;
	btCollisionDispatcher* dispatcher;
public:
	btTransform trans;
	btTransform boxTransform;

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	btDiscreteDynamicsWorld* dynamicsWorld;

	PhysicsEngine();
	void CreateGround();
	void AddRigidBody(btCollisionShape* colShape);
	void Solve(float deltaTime);
	void Destroy();

};

