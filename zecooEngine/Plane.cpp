#include "Plane.h"

Plane::Plane(Material* _material, Texture* _texture) : Model(_material, _texture)
{
	planeMesh = new PlaneMesh();
}

void Plane::SetRigidbody( PhysicsEngine* physicsEngine )
{
	//create a dynamic rigidbody
	btCollisionShape* groundShape = new btBoxShape( btVector3( btScalar( transform->pose[0][0]/2 ), btScalar( transform->pose[1][1]/2 ), btScalar( transform->pose[2][2] /2) ) );
	physicsEngine->collisionShapes.push_back( groundShape );

	/// Create Dynamic Objects
	btTrans.setIdentity();
    //btTrans.setFromOpenGLMatrix( glm::value_ptr(transform->pose ) );
	//glm::vec3 pos = transform->getPosition();
	//btTrans.setOrigin( btVector3( pos.x, pos.y, pos.z ) );

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = ( mass != 0.f );

	btVector3 localInertia( 0, 0, 0 );

	if( isDynamic )
		groundShape->calculateLocalInertia( mass, localInertia );

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState( btTrans );
	btRigidBody::btRigidBodyConstructionInfo rbInfo( mass, myMotionState, groundShape, localInertia );
	//rbInfo.m_restitution = 1.0;
	rigidBody = new btRigidBody( rbInfo );

	isRigidBody = true;

	physicsEngine->dynamicsWorld->addRigidBody( rigidBody );
}

glm::mat4 btScalar2mat4( btScalar* matrix )
{
	return glm::mat4(
		matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10], matrix[11],
		matrix[12], matrix[13], matrix[14], matrix[15] );
}

void Plane::solve( PhysicsEngine* physicsEngine )
{
	if( rigidBody && rigidBody->getMotionState() )
		rigidBody->getMotionState()->getWorldTransform( btTrans );

	/*glm::mat4 openGLmatrix;
	btTrans.getOpenGLMatrix( glm::value_ptr( openGLmatrix ) );
	transform->position(glm::vec3(btTrans.getOrigin().getX(), btTrans.getOrigin().getY(), btTrans.getOrigin().getZ()));
	transform->worldMatrix = openGLmatrix;
	transform->Update();*/

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
