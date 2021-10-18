#include "Cube.h"

Cube::Cube(Material* _material, Texture* _texture):Model(_material, _texture)
{
	cubeMesh = new CubeMesh();
}

void Cube::SetRigidbody(PhysicsEngine* physicsEngine)
{
	//create a dynamic rigidbody
	btCollisionShape* colShape = new btBoxShape(btVector3( transform->pose[0][0]/2, transform->pose[0][0] / 2, transform->pose[0][0] / 2 ));
	physicsEngine->collisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	btTrans.setIdentity();

	glm::vec3 pos = transform->getPosition();
	btTrans.setOrigin( btVector3( pos.x, pos.y, pos.z ) );

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);

	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(btTrans);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	rigidBody = new btRigidBody(rbInfo);

	isRigidBody = true;

	physicsEngine->dynamicsWorld->addRigidBody(rigidBody);
}

void Cube::solve(PhysicsEngine* physicsEngine)
{
	if (rigidBody && rigidBody->getMotionState())
		rigidBody->getMotionState()->getWorldTransform(btTrans);

	glm::mat4 ogl_t;
	btTrans.getOpenGLMatrix(glm::value_ptr(ogl_t));
	transform->position(glm::vec3(btTrans.getOrigin().getX(), btTrans.getOrigin().getY(), btTrans.getOrigin().getZ()));
	transform->Update();

	//printf("world pos object %d = %f,%f,%f\n", 1, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
}

void Cube::render()
{
	Model::render();
	cubeMesh->Render();
}

Cube::~Cube()
{

}
