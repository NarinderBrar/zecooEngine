#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "CustomMesh.h";
#include "Transform.h";

#include "btBulletDynamicsCommon.h";
#include "PhysicsEngine.h";

class Sphere :public Model
{
private:
	CustomMesh* customMesh;

public:
	bool isRigidBody = false;
	btScalar mass;

	btTransform btTrans;
	btCollisionShape* btColShape;
	btRigidBody* rigidBody;

	Sphere(Material* material, Texture* texture);

	void SetRigidbody( PhysicsEngine* physicsEngine );

	void solve( PhysicsEngine* physicsEngine );

	void render();

	~Sphere();
};

