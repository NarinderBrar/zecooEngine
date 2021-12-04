#include "Cube.h"

Cube::Cube(Material* _material, Texture* _texture):Model(_material, _texture)
{
	cubeMesh = new CubeMesh();
}

void Cube::SetRigidbody(PhysicsEngine* physicsEngine)
{
	//create a dynamic rigidbody
	btCollisionShape* colShape = new btBoxShape( btVector3( transform->localScale.x / 2.0, transform->localScale.y / 2.0, transform->localScale.z / 2.0 ) );
	physicsEngine->collisionShapes.push_back( colShape );

	/// Create Dynamic Objects
	btTrans.setIdentity();
	//btTrans.setFromOpenGLMatrix( glm::value_ptr( transform->pose ) );

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = ( mass != 0.f );

	btVector3 localInertia( 0, 0, 0 );

	if( isDynamic )
	{
		btTrans.setFromOpenGLMatrix( glm::value_ptr( transform->pose ) );
		colShape->calculateLocalInertia( mass, localInertia );
	}
	else
	{
		btQuaternion orn;
		orn.setEuler( transform->eulerAngle.y, transform->eulerAngle.x, transform->eulerAngle.z );
		btTrans.setRotation( orn );

		btTrans.setOrigin( btVector3( transform->getPosition().x, transform->getPosition().y, transform->getPosition().z ) );
	}

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState( btTrans );
	btRigidBody::btRigidBodyConstructionInfo rbInfo( mass, myMotionState, colShape, localInertia );
	//rbInfo.m_restitution = 1.0;
	rigidBody = new btRigidBody( rbInfo );

	isRigidBody = true;

	physicsEngine->dynamicsWorld->addRigidBody( rigidBody );
}

void Cube::solve(PhysicsEngine* physicsEngine)
{
	if (rigidBody && rigidBody->getMotionState())
		rigidBody->getMotionState()->getWorldTransform(btTrans);

	glm::mat4 openGLmatrix;
	btTrans.getOpenGLMatrix(glm::value_ptr( openGLmatrix ));
	transform->worldMatrix = glm::scale( openGLmatrix, transform->localScale );
	transform->Update();
}

void Cube::render()
{
	Model::render();
	cubeMesh->Render();
}

Cube::~Cube()
{

}
