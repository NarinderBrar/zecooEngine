#include "Plane.h"

Plane::Plane(Material* _material, Texture* _texture) : Model(_material, _texture)
{
	planeMesh = new PlaneMesh();
}

void Plane::SetRigidbody( PhysicsEngine* physicsEngine )
{
	//create a dynamic rigidbody
	btCollisionShape* groundShape = new btBoxShape( btVector3( btScalar( transform->pose[0][0] ), btScalar( 0.0 ), btScalar( transform->pose[2][2] ) ) );
	physicsEngine->collisionShapes.push_back( groundShape );

	/// Create Dynamic Objects
	btTrans.setIdentity();

	glm::vec3 pos = transform->getPosition();
	btTrans.setOrigin( btVector3( pos.x, pos.y, pos.z ) );

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = ( mass != 0.f );

	btVector3 localInertia( 0, 0, 0 );

	if( isDynamic )
		groundShape->calculateLocalInertia( mass, localInertia );

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState( btTrans );
	btRigidBody::btRigidBodyConstructionInfo rbInfo( mass, myMotionState, groundShape, localInertia );
	rigidBody = new btRigidBody( rbInfo );

	isRigidBody = true;

	physicsEngine->dynamicsWorld->addRigidBody( rigidBody );
}

void Plane::solve( PhysicsEngine* physicsEngine )
{
	if( rigidBody && rigidBody->getMotionState() )
		rigidBody->getMotionState()->getWorldTransform( btTrans );

	glm::mat4 ogl_t;
	btTrans.getOpenGLMatrix( glm::value_ptr( ogl_t ) );
	transform->position( glm::vec3( btTrans.getOrigin().getX(), btTrans.getOrigin().getY(), btTrans.getOrigin().getZ() ) );
	transform->Update();

	//printf("world pos object %d = %f,%f,%f\n", 1, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
}

void Plane::render()
{
	Model::render();
	planeMesh->Render();
}

Plane::~Plane()
{
}
