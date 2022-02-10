#include "Load.h"
namespace A
{
	Node* MakeRoom(int data, PointNode* p)
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


	Node* InsertVerticesOrIndices(Node* root, int data, PointNode* p)
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
	void printIndices(struct Node* node, int d, btConvexHullShape** convex)
	{
		if (node == NULL)
			return;

		if (*node->data == d)
		{
			(*convex)->addPoint(btVector3(*node->point->x, *node->point->y, *node->point->z));

			//cout<< *node->point->x <<"  "<< *node->point->y <<"  "<< *node->point->z <<endl;
			return;
		}



		printIndices(node->left, d, convex);


		printIndices(node->right, d, convex);
	}
	void printVertices(struct Node* node, struct Node* rootVertices, int d, btConvexHullShape** convex)
	{
		if (node == NULL)
			return;

		if (*node->data == d)
		{
			printIndices(rootVertices, *node->point->x, convex);
			printIndices(rootVertices, *node->point->y, convex);
			printIndices(rootVertices, *node->point->z, convex);

			return;
		}



		printVertices(node->left, rootVertices, d, convex);


		printVertices(node->right, rootVertices, d, convex);
	}
	void LoadMe(btConvexHullShape** convex, vector<float>& vertices, vector<int>& indices,PointForEq* p1)
	{
		float mass = 1.0f;
		int pcount = 0, icount = 0, uvcount = 0;
		PointNode* p = new PointNode();
		Node* rootVertices = NULL;
		//rootVertices=InsertVerticesOrIndices(rootVertices, 1, p);
		Node* rootIndices = NULL;
		Node* rootUVs = NULL;
		vector<float> v;
		string myText;
		ifstream MyReadFile("A1.obj"); //reader
		int i = 0, j = -1, k = 0;
		cout << " " << (*p1).z1 << "  " << (*p1).z2 << "  " << (*p1).z3 << "  " << (*p1).z4 << endl;
		//(*convex)->
		//(*convex)->setMargin(0.15);
		//btTransform t;
		//t.setIdentity();
		//t.setOrigin(btVector3(0, 0, 0));
		//btCompoundShape* compound = new btCompoundShape();
		//compound->addChildShape(t, *convex);
		//btVector3 inertia(0, 0, 0);
		//btScalar masses[2] = { mass,mass / 2 };
		//compound->calculatePrincipalAxisTransform(masses, t, inertia);
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
						*p->x = (*p1).x1; *p->y = (*p1).y1; *p->z = (*p1).z1; //-
		cout << " " << *p->x << "  " << *p->y << "  " << *p->z << endl;
					}
					if (i == 4)
					{
						*p->x = (*p1).x4; *p->y = (*p1).y4; *p->z = (*p1).z4; //-
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
						*p->x = (*p1).x2; *p->y = (*p1).y2; *p->z = (*p1).z2; //-
		cout << " " << *p->x << "  " << *p->y << "  " << *p->z << endl;
					}
					if (i == 8)
					{
						
						*p->x = (*p1).x3; *p->y = (*p1).y3; *p->z = (*p1).z3; //-
		cout << " " << *p->x << "  " << *p->y << "  " << *p->z << endl;
					}
					*p->z = *p->z/6.0f;
					rootVertices = InsertVerticesOrIndices(rootVertices, i, p);
				}

				if (myText.rfind("f ", 0) == 0) {
					j++; icount++;
					string s = myText.substr(2, myText.length());
					const int n = s.length();
					char* char_array = new char[n + 1];
					PointNode* p = new PointNode();
					p->x = new float(); p->y = new float(); p->z = new float();
					strcpy(char_array, s.c_str());
					int t;
					sscanf(char_array, "%f/%d/%d %f/%d/%d %f/%d/%d", &(*p->x), &t, &t, &(*p->y), &t, &t, &(*p->z), &t, &t);

					rootIndices = InsertVerticesOrIndices(rootIndices, j, p);
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
			printVertices(rootIndices, rootVertices, k, convex);

		}

		//0 0 1 2
		//1 3 4 5


		//return v;
	}
}

