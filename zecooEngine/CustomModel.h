#pragma once

#include "Model.h";
#include "Shader.h";
#include "Texture.h";
#include "CustomMesh.h";
#include "Transform.h";

#include "btBulletDynamicsCommon.h";
#include "PhysicsEngine.h";

class CustomModel :public Model
{
private:
	CustomMesh* customMesh;

public:
	bool isRigidBody = false;
	btScalar mass;

	btTransform btTrans;
	btCollisionShape* btColShape;
	btRigidBody* rigidBody;

	CustomModel(Material* material, Texture* texture);

	void SetRigidbody( PhysicsEngine* physicsEngine );

	void solve( PhysicsEngine* physicsEngine );

	void render();

	~CustomModel();
};

