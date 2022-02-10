#pragma once
#include "Model.h";
#include "Material.h";
#include "Texture.h";
#include "BtConvexHullShapeBlenderMesh.h"
#include "Transform.h";
#include "PointForEq.h"
#include "Input.h"
#include "btBulletDynamicsCommon.h";
#include "PhysicsEngine.h";
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include<cstring>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <glm/glm.hpp>
#include "Load.h"
using namespace std;
typedef double coord_t;
typedef double coord2_t;
namespace Blender
{
	struct Point {
		coord_t x, y;

		bool operator <(const Point& p) const {
			return x < p.x || (x == p.x && y < p.y);
		}
	};
	struct Point1 {
		coord_t x, y, z;

		bool operator <(const Point& p) const {
			return x < p.x || (x == p.x && y < p.y);
		}
	};
	struct Point3 {
		float x, y, z;
	};
	struct Index {
		int x, y, z;
	};
}

class BtConvexHullShapeBlender :public Model
{
private:
	BtConvexHullShapeBlenderMesh* cubeMesh;
public:
	string pathToobj;
	bool isRigidBody = false;
	btScalar mass;
	vector<Blender::Point1> v, v1;
	float vertices2[36];
	btTransform btTrans;
	btCollisionShape* btColShape;
	btRigidBody* rigidBody;
	vector<float> vertices;
	vector<float> vertices1;
	vector<float> indices;
	PointForEq* p;
	//BtConvexHullShapeBlender(Material* material, Texture* texture, string s, string s1);
	BtConvexHullShapeBlender(Material* material, Texture* texture, string s, PointForEq* p);
	BtConvexHullShapeBlender(Material* material, Texture* texture);
	void CalculatePoints();
	void SetRigidbody(PhysicsEngine* physicsEngine);
	void drawIt();
	void addIndexReal(int a, int b, int c);
	void solve(PhysicsEngine* physicsEngine);
	coord2_t cross(const Blender::Point& O, const Blender::Point& A, const Blender::Point& B);
	vector<Blender::Point> convex_hull(vector<Blender::Point> P);
	void addI(int a, int b, int c);
	void render();
	void Load(vector<float>& f);
	void ico();
	void addVertex(const float v1[3], const float v2[3], const float newV[3]);
	void computeHalfVertex(const float v1[3], const float v2[3], float newV[3]);
	void addIndex(int a, int b, int c);
	Input* input;
	~BtConvexHullShapeBlender();
};






