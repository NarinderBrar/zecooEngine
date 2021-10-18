#pragma once

#include "Model.h";
#include "Material.h";
#include "Texture.h";
#include "PlaneMesh.h";
#include "Transform.h";

#include "btBulletDynamicsCommon.h";
#include "PhysicsEngine.h";

class Plane:public Model
{
	private:
		PlaneMesh* planeMesh;

	public:
		bool isRigidBody = false;
		btScalar mass = 0.0;

		btTransform btTrans;
		btCollisionShape* btColShape;
		btRigidBody* rigidBody;

		Plane(Material* _material, Texture* _texture);

		void SetRigidbody( PhysicsEngine* physicsEngine );

		void solve( PhysicsEngine* physicsEngine );

		void render();

		~Plane();
};

