#include "PhysicsEngine.h"

void myTickCallback( btDynamicsWorld* dynamicsWorld, btScalar timeStep )
{
	//objectsCollisions.clear();

	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
	for( int i = 0; i < numManifolds; i++ )
	{
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal( i );
		auto* objA = contactManifold->getBody0();
		auto* objB = contactManifold->getBody1();

		//auto& collisionsA = objectsCollisions[objA];
		//auto& collisionsB = objectsCollisions[objB];

		int numContacts = contactManifold->getNumContacts();
		for( int j = 0; j < numContacts; j++ )
		{
			btManifoldPoint& pt = contactManifold->getContactPoint( j );
			//collisionsA.push_back( &pt );
			//collisionsB.push_back( &pt );

			//cout << "h" << endl;
		}
	}
}

PhysicsEngine::PhysicsEngine()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	//dynamicsWorld->setInternalTickCallback( myTickCallback );
}

void PhysicsEngine::Solve(float deltaTime)
{
	//dynamicsWorld->debugDrawWorld();
	dynamicsWorld->stepSimulation(deltaTime);
}

bool PhysicsEngine::CollisionTest( std::string bodyA, std::string bodyB )
{
	const int numManifolds = dispatcher->getNumManifolds();

	for( int m = 0; m < numManifolds; ++m )
	{
		btPersistentManifold* man = dispatcher->getManifoldByIndexInternal( m );

		const btRigidBody* rb1 = static_cast<const btRigidBody*>( man->getBody0() );
		const btRigidBody* rb2 = static_cast<const btRigidBody*>( man->getBody1() );

		const Model* rb1M = (Model*)rb1->getUserPointer();
		const Model* rb2M = (Model*)rb2->getUserPointer();

		if( ( rb1M->name != bodyA && rb2M->name != bodyB ) || ( rb1M->name != bodyB || rb2M->name != bodyA ) )
		{
			float totalImpact = 0.0f;
			for( int c = 0; c < man->getNumContacts(); ++c )
				totalImpact += man->getContactPoint( c ).m_appliedImpulse;

			if( totalImpact > 0 )
				return true;
		}
	}

	return false;
}

void PhysicsEngine::Destroy()
{
	///-----stepsimulation_end-----
	//cleanup in the reverse order of creation/initialization
	///-----cleanup_start-----
	//remove the rigidbodies from the dynamics world and delete them

	int i;

	for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}