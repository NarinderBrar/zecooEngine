#include "LoaderForGraph.h"
Node* LoaderForGraph::MakeRoom(int data, PointNode* p)
{
	Node* newNode = new Node();
	if (!newNode) {
		return NULL;
	}
	newNode->data = new int();
	*newNode->data = data;
	newNode->point = p;
	newNode->left = newNode->right = NULL;
	return newNode;
}
LoaderForGraph::LoaderForGraph(string s)
{
	strPath = s;
}

Node* LoaderForGraph::InsertVerticesOrIndices(Node* root, int data, PointNode* p)
{
	if (root == NULL) {
		root = MakeRoom(data, p);
		return root;
	}

	queue<Node*> q;
	q.push(root);

	while (!q.empty()) {
		Node* temp = q.front();
		q.pop();

		if (temp->left != NULL)
			q.push(temp->left);
		else {
			temp->left = MakeRoom(data, p);
			return root;
		}

		if (temp->right != NULL)
			q.push(temp->right);
		else {
			temp->right = MakeRoom(data, p);
			return root;
		}
	}
}
void LoaderForGraph::printIndices(struct Node* node, int d, vector<float>& v)
{
	if (node == NULL)
		return;

	if (*node->data == d)
	{
		//(*convex)->addPoint(btVector3(*node->point->x, *node->point->y, *node->point->z));
		v.push_back(*node->point->x);
		v.push_back(*node->point->y);
		v.push_back(*node->point->z);

		return;
	}



	printIndices(node->left, d, v);


	printIndices(node->right, d, v);
}
void LoaderForGraph::printVertices(struct Node* node, struct Node* rootVertices, int d, vector<float>& v)
{
	if (node == NULL)
		return;

	if (*node->data == d)
	{
		printIndices(rootVertices, *node->point->x, v);
		printIndices(rootVertices, *node->point->y, v);
		printIndices(rootVertices, *node->point->z, v);

		return;
	}



	printVertices(node->left, rootVertices, d, v);


	printVertices(node->right, rootVertices, d, v);
}
void LoaderForGraph::printUVVertices(struct Node* node, int d, vector<float>& v, bool flag)
{
	if (node == NULL)
		return;

	if (*node->data == d)
	{
		//(*convex)->addPoint(btVector3(*node->point->x, *node->point->y, *node->point->z));
		v.push_back(*node->point->x);
		v.push_back(*node->point->y);  if (!flag)  v.push_back(*node->point->z);
		//v.push_back(*node->point->z);
		//cout << " x " << *node->point->x << " y " << *node->point->y << endl;
	//	if (!flag) 	cout << "normals -  x  " << *node->point->x << "  y  " << *node->point->y << "  z  " << *node->point->z << endl;
	//	else cout << "uvs -  x  " << *node->point->x << "  y  " << *node->point->y << endl;

		return;
	}



	printUVVertices(node->left, d, v, flag);


	printUVVertices(node->right, d, v, flag);
}
void LoaderForGraph::printUVIndices(struct Node* node, struct Node* rootVertices, int d, vector<float>& v, bool flag)
{
	if (node == NULL)
		return;

	if (*node->data == d)
	{
		//if (!flag) 	cout << "normals INdices -  x  " << *node->point->x << "  y  " << *node->point->y << "  z  " << *node->point->z << endl;
		//else cout << "uvs Indices -  x  " << *node->point->x << "  y  " << *node->point->y << endl;
		printUVVertices(rootVertices, *node->point->x, v, flag);
		printUVVertices(rootVertices, *node->point->y, v, flag);
		printUVVertices(rootVertices, *node->point->z, v, flag);
		return;
	}



	printUVIndices(node->left, rootVertices, d, v, flag);


	printUVIndices(node->right, rootVertices, d, v, flag);
}
void LoaderForGraph::printV(struct Node* node, int d, vector<float>& v)
{
	if (node == NULL)
		return;

	if (*node->data == d)
	{
		//cout << "x  " << *node->point->x << "  y  " << *node->point->y<<"  z  " << *node->point->z<<endl;
		v.push_back(*node->point->x);
		v.push_back(*node->point->y);
		v.push_back(*node->point->z);
		return;
	}



	printV(node->left, d, v);


	printV(node->right, d, v);
}
void LoaderForGraph::printV1(struct Node* node, int d, vector<int>& v)
{
	if (node == NULL)
		return;

	if (*node->data == d)
	{
		//	cout << "x  " << *node->point->x << "  y  " << *node->point->y << "  z  " << *node->point->z << endl;
		v.push_back(*node->point->x);
		v.push_back(*node->point->y);
		v.push_back(*node->point->z);
		return;
	}



	printV1(node->left, d, v);


	printV1(node->right, d, v);
}
void LoaderForGraph::LoadMe(btConvexHullShape** convex, vector<float>& vertices, vector<float>& normals, vector<float>& uvs, vector<int>& indices, PointForEq* p1)
{

	PointNode* p = new PointNode();

	vector<float> v;
	string myText;
	ifstream MyReadFile(strPath);

	if (convex != NULL)
	{
		(*convex)->setMargin(0);
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(0, 0, 0));
		btCompoundShape* compound = new btCompoundShape();
		compound->addChildShape(t, *convex);
		btVector3 inertia(0, 0, 0);
		btScalar masses[2] = { mass,mass / 2 };
		compound->calculatePrincipalAxisTransform(masses, t, inertia);
	}
	if (vertices.size() == 0 && indices.size() == 0)
	{
		while (getline(MyReadFile, myText))
		{

			if (myText.rfind("v ", 0) == 0) {
				i++; pcount++;
				string s = myText.substr(2, myText.length());
				const int n = s.length();
				char* char_array = new char[n + 1];
				strcpy(char_array, s.c_str());
				PointNode* p = new PointNode();
				p->x = new float(); p->y = new float(); p->z = new float();
				//cout << "x  " << *p->x << "  y  " << *p->y << "  z  " << *p->z << endl;

				sscanf(char_array, "%f %f %f", &*p->x, &*p->y, &*p->z);
				if (i == 1)
				{
					//cout << "zzzzzzzz";
					//*p->x = (*p1).x2; *p->y = (*p1).y2; *p->z = -(*p1).z2;
					*p->x = (*p1).x1; *p->y = (*p1).y1; *p->z = (*p1).z1;
					cout << " " << *p->x << "  " << *p->y << "  " << *p->z << endl;
				}
				if (i == 2)
				{
					*p->x = (*p1).x4; *p->y = (*p1).y4; *p->z = (*p1).z4;
					cout << " " << *p->x << "  " << *p->y << "  " << *p->z << endl;
				}
				if (i == 3)
				{
					//*p->x = (*p1).x2; *p->y = (*p1).y2; *p->z = (*p1).z2;
					*p->x = (*p1).x1; *p->y = (*p1).y1; *p->z = -(*p1).z1; //-
					cout << " " << *p->x << "  " << *p->y << "  " << *p->z << endl;
				}
				if (i == 4)
				{
					*p->x = (*p1).x4; *p->y = (*p1).y4; *p->z = -(*p1).z4; //-
					cout << " " << *p->x << "  " << *p->y << "  " << *p->z << endl;

				}
				if (i == 5)
				{
					*p->x = (*p1).x2; *p->y = (*p1).y2; *p->z = (*p1).z2;
					cout << " " << *p->x << "  " << *p->y << "  " << *p->z << endl;
				}
				if (i == 6)
				{
					*p->x = (*p1).x3; *p->y = (*p1).y3; *p->z = (*p1).z3;
					cout << " " << *p->x << "  " << *p->y << "  " << *p->z << endl;
				}
				if (i == 7)
				{
					*p->x = (*p1).x2; *p->y = (*p1).y2; *p->z = -(*p1).z2; //-
					cout << " " << *p->x << "  " << *p->y << "  " << *p->z << endl;
				}
				if (i == 8)
				{

					*p->x = (*p1).x3; *p->y = (*p1).y3; *p->z = -(*p1).z3; //-
					cout << " " << *p->x << "  " << *p->y << "  " << *p->z << endl;
				}
				*p->z = *p->z / 3.0f;
				rootVertices = InsertVerticesOrIndices(rootVertices, i, p);
			}
			if (myText.rfind("vt ", 0) == 0) {
				k++; uvcount++;
				string s = myText.substr(3, myText.length());
				const int n = s.length();
				char* char_array = new char[n + 1];
				strcpy(char_array, s.c_str());
				PointNode* p = new PointNode();
				p->x = new float(); p->y = new float();
				sscanf(char_array, "%f %f", &*p->x, &*p->y);

				rootUVs = InsertVerticesOrIndices(rootUVs, k, p);
				//uvs.push_back(*p->x); uvs.push_back(*p->y);
			//	cout << "uvs " << *p->x << *p->y;
			}
			if (myText.rfind("vn ", 0) == 0) {
				//cout << "normals calllllled";
				g++; normalcount++;
				string s = myText.substr(3, myText.length());
				const int n = s.length();
				char* char_array = new char[n + 1];
				strcpy(char_array, s.c_str());
				PointNode* p = new PointNode();
				p->x = new float(); p->y = new float(); p->z = new float();

				sscanf(char_array, "%f %f %f", &*p->x, &*p->y, &*p->z);


				rootNormals = InsertVerticesOrIndices(rootNormals, g, p);
				//uvs.push_back(*p->x); uvs.push_back(*p->y);
			//	cout << "uvs " << *p->x << *p->y;
			}
			if (myText.rfind("f ", 0) == 0) {
				j++; icount++;
				string s = myText.substr(2, myText.length());
				const int n = s.length();
				char* char_array = new char[n + 1];
				PointNode* p = new PointNode();
				p->x = new float(); p->y = new float(); p->z = new float();
				PointNode* p1 = new PointNode();
				p1->x = new float(); p1->y = new float(); p1->z = new float();
				PointNode* p2 = new PointNode();
				p2->x = new float(); p2->y = new float(); p2->z = new float();
				strcpy(char_array, s.c_str());
				int t;
				sscanf(char_array, "%f/%f/%f %f/%f/%f %f/%f/%f", &(*p->x), &(*p1->x), &(*p2->x), &(*p->y), &(*p1->y), &(*p2->y), &(*p->z), &(*p1->z), &(*p2->z));

				rootIndices = InsertVerticesOrIndices(rootIndices, j, p);
				rootUVIndices = InsertVerticesOrIndices(rootUVIndices, j, p1);
				rootNormalIndices = InsertVerticesOrIndices(rootNormalIndices, j, p2);
				//	cout << " Reading indices        " << (*p2->x)<<"   " << (*p2->y) << "   " << (*p2->z) <<endl;
			}

		}
	}
	else
	{

		for (int k = 0;k < vertices.size();k += 3)
		{
			i++;
			PointNode* p = new PointNode();
			p->x = new float(); p->y = new float(); p->z = new float();
			*p->x = vertices[k];
			*p->y = vertices[k + 1];
			*p->z = vertices[k + 2];
			pcount++;
			rootVertices = InsertVerticesOrIndices(rootVertices, i, p);
		}
		for (int k = 0;k < indices.size();k += 3)
		{
			j++;
			PointNode* p = new PointNode();
			p->x = new float(); p->y = new float(); p->z = new float();
			*p->x = indices[k];
			*p->y = indices[k + 1];
			*p->z = indices[k + 2];
			icount++;
			rootIndices = InsertVerticesOrIndices(rootIndices, j, p);
		}
	}

	for (int k = 0;k <= icount;k++)
	{
		//	printVertices(rootIndices, rootVertices, k, convex);
		printVertices(rootIndices, rootVertices, k, vertices);
		printUVIndices(rootUVIndices, rootUVs, k, uvs, true);
		printUVIndices(rootNormalIndices, rootNormals, k, normals, false);
	}

	//0 0 1 2
	//1 3 4 5


	//return v;

}
void LoaderForGraph::LoadData(vector<float>& vertices, vector<int>& indices)
{
	for (int i = 0;i < pcount;i++)
	{
		//cout << "calling "<<endl;
		printV(rootVertices, i + 1, vertices);
	}

	for (int i = 0;i < icount;i++)
	{
		printV1(rootIndices, i, indices);
	}
}
void LoaderForGraph::LoadData1(vector<float>& vertices, vector<int>& indices)
{
	for (int k = 0;k <= icount;k++)
	{
		printVertices(rootIndices, rootVertices, k, vertices);

	}
}


