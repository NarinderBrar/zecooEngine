#pragma once
#include "Model.h";
#include "Material.h";
#include "Texture.h";
#include "CubeMesh.h"
#include "Transform.h";

#include "btBulletDynamicsCommon.h";
#include "PhysicsEngine.h";

class Cube:public Model
{
private:
	CubeMesh* cubeMesh;
public:
	bool isRigidBody = false;

	btTransform btTrans;
	btCollisionShape* btColShape;
	btRigidBody* rigidBody;

	Cube(Material* material, Texture* texture);

	void SetRigidbody(PhysicsEngine* physicsEngine);

	void solve(PhysicsEngine* physicsEngine);

	void render();

	~Cube();
};

