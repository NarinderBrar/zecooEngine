#pragma once
#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <iostream>
#include "Model.h";
#include "PointForEq.h";
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
#include <cstring>
using namespace std;
struct PointNode {
	float* x, * y, * z;
};
struct Node {
	int* data;
	Node* left;
	Node* right;
	PointNode* point;
};
class LoaderForGraph
{
public:
	Node* rootVertices = NULL;
	Node* rootUVs = NULL;
	Node* rootUVIndices = NULL;
	Node* rootNormals = NULL;
	Node* rootNormalIndices = NULL;
	//rootVertices=InsertVerticesOrIndices(rootVertices, 1, p);
	Node* rootIndices = NULL;
	float mass = 1.0f;
	string strPath;
	int pcount = 0, icount = 0, uvcount = 0;
	int i = 0, j = -1, k = 0, g = 0, normalcount = 0;
	LoaderForGraph(string s);
	Node* MakeRoom(int data, PointNode* p);
	Node* InsertVerticesOrIndices(Node* root, int data, PointNode* p);
	void printUVVertices(struct Node* node, int d, vector<float>& v, bool flag);
	void printUVIndices(struct Node* node, struct Node* rootVertices, int d, vector<float>& v, bool flag);
	void printIndices(struct Node* node, int d, vector<float>& v);
	void printV(struct Node* node, int d, vector<float>& v);
	void printVertices(struct Node* node, struct Node* rootVertices, int d, vector<float>& v);
	void LoadMe(btConvexHullShape** convex, vector<float>& vertices, vector<float>& normals, vector<float>& uvs, vector<int>& indices, PointForEq* p1);
	void LoadConvex(btConvexHullShape**);
	void printV1(struct Node* node, int d, vector<int>& v);
	void LoadData(vector<float>& vertices, vector<int>& indice);
	void LoadData1(vector<float>& vertices, vector<int>& indice);
};






