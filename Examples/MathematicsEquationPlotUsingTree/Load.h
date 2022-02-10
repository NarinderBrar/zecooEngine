#pragma once

#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <iostream>
#include "Model.h";
#include "Material.h";
#include "Texture.h";
#include "CubeMesh.h"
#include "Transform.h";
#include "Input.h"
#include "btBulletDynamicsCommon.h";
#include "PhysicsEngine.h";
#include <fstream>
#include<cstring>
#include <vector>
#include <stdio.h>
#include <string>
#include "PointForEq.h"
#include <cstring>
using namespace std;
namespace A {
	struct PointNode {
		float* x, * y, * z;
	};
	struct Node {
		int* data;
		Node* left;
		Node* right;
		PointNode* point;
	};
	void LoadMe(btConvexHullShape** convex, vector<float>& vertices, vector<int>& indices,PointForEq* p);
	void LoadConvex(btConvexHullShape**);

}





