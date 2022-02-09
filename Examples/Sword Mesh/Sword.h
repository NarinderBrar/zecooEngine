#pragma once
#include "Model.h";
#include "Material.h";
#include "Texture.h";
#include "SwordMesh.h"
#include "Transform.h";

#include "btBulletDynamicsCommon.h";
#include "PhysicsEngine.h";

class Sword :public Model
{
private:
	SwordMesh* swordMesh;
public:
	bool isRigidBody = false;

	btTransform btTrans;
	btCollisionShape* btColShape;
	btRigidBody* rigidBody;

	Sword(Material* material, Texture* texture);

	void SetRigidbody(PhysicsEngine* physicsEngine);

	void solve(PhysicsEngine* physicsEngine);

	void render();

	~Sword();
};

