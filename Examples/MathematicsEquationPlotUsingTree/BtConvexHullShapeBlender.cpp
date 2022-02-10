#include "BtConvexHullShapeBlender.h"
#include "BtConvexHullShapeBlender.h"
BtConvexHullShapeBlender::BtConvexHullShapeBlender(Material* _material, Texture* _texture, string s1, PointForEq* p1) :Model(_material, _texture)
{
	cubeMesh = new BtConvexHullShapeBlenderMesh(s1,p1);
	//pathToobj = s;
	//cout << " " << (*p1).z1 << "  " << (*p1).z2 << "  " << (*p1).z3 << "  " << (*p1).z4 << endl;
	p = p1;
}
coord2_t BtConvexHullShapeBlender::cross(const Blender::Point& O, const Blender::Point& A, const Blender::Point& B)
{
	return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}
vector<Blender::Point> BtConvexHullShapeBlender::convex_hull(vector<Blender::Point> P)
{
	size_t n = P.size(), k = 0;
	if (n <= 3) return P;
	vector<Blender::Point> H(2 * n);
	sort(P.begin(), P.end());
	for (size_t i = 0; i < n; ++i) {
		while (k >= 2 && cross(H[k - 2], H[k - 1], P[i]) <= 0) k--;
		H[k++] = P[i];
	}

	for (size_t i = n - 1, t = k + 1; i > 0; --i) {
		while (k >= t && cross(H[k - 2], H[k - 1], P[i - 1]) <= 0) k--;
		H[k++] = P[i - 1];
	}

	H.resize(k - 1);
	return H;
}
void BtConvexHullShapeBlender::SetRigidbody(PhysicsEngine* physicsEngine)
{
	//btConcaveShape* convexShape = new btConcaveShape();
	//btCollisionShape *c = new btCollisionShape();
	btCompoundShape* s = new btCompoundShape();
	btTransform t;
	btConvexHullShape* convex = new btConvexHullShape(); // Construct a convex collision model using the point set of the table, although the table is concave
	//s->addChildShape(t, convexShape);
	vector<float> v;
	vector<int> i;
	//A::LoadMe(&convex, v, i, p);
	//cout << " " << (*p).z1<< "  " << (*p).z2 << "  " << (*p).z3 << "  " << (*p).z4 << endl;

	/*convex->addPoint(btVector3(5.391068, 1.000000, 1.368849));
	convex->addPoint(btVector3(5.391068, -1.000000, -0.824488));
	convex->addPoint(btVector3(5.391068, -1.000000, 1.368849));
	convex->addPoint(btVector3(5.391068, 1.000000, -0.824488));
	convex->addPoint(btVector3(7.387821, -1.000000, -0.824488));
	convex->addPoint(btVector3(5.391068, -1.000000, -0.824488));
	convex->addPoint(btVector3(7.387821, 1.000000, -0.824488));
	convex->addPoint(btVector3(7.387821, -1.000000, 1.368849));
	convex->addPoint(btVector3(7.387821, -1.000000, -0.824488));
	convex->addPoint(btVector3(7.387821, 1.000000, 1.368849));
	convex->addPoint(btVector3(5.391068, -1.000000, 1.368849));
	convex->addPoint(btVector3(7.387821, -1.000000, 1.368849));
	convex->addPoint(btVector3(7.387821, -1.000000, -0.824488));
	convex->addPoint(btVector3(5.391068, -1.000000, 1.368849));
	convex->addPoint(btVector3(5.391068, -1.000000, -0.824488));
	convex->addPoint(btVector3(5.391068, 1.000000, -0.824488));
	convex->addPoint(btVector3(7.387821, 1.000000, 1.368849));
	convex->addPoint(btVector3(7.387821, 1.000000, -0.824488));
	convex->addPoint(btVector3(5.391068, 1.000000, 1.368849));
	convex->addPoint(btVector3(5.391068, 1.000000, -0.824488));
	convex->addPoint(btVector3(5.391068, -1.000000, -0.824488));
	convex->addPoint(btVector3(5.391068, 1.000000, -0.824488));
	convex->addPoint(btVector3(7.387821, 1.000000, -0.824488));
	convex->addPoint(btVector3(7.387821, -1.000000, -0.824488));
	convex->addPoint(btVector3(7.387821, 1.000000, -0.824488));
	convex->addPoint(btVector3(7.387821, 1.000000, 1.368849));
	convex->addPoint(btVector3(7.387821, -1.000000, 1.368849));
	convex->addPoint(btVector3(7.387821, 1.000000, 1.368849));
	convex->addPoint(btVector3(5.391068, 1.000000, 1.368849));
	convex->addPoint(btVector3(5.391068, -1.000000, 1.368849));
	convex->addPoint(btVector3(7.387821, -1.000000, -0.824488));
	convex->addPoint(btVector3(7.387821, -1.000000, 1.368849));
	convex->addPoint(btVector3(5.391068, -1.000000, 1.368849));
	convex->addPoint(btVector3(5.391068, 1.000000, -0.824488));
	convex->addPoint(btVector3(5.391068, 1.000000, 1.368849));
	convex->addPoint(btVector3(7.387821, 1.000000, 1.368849));/**/
	btCollisionShape* colShape = new btBoxShape(btVector3(transform->pose[0][0] / 2, transform->pose[0][0] / 2, transform->pose[0][0] / 2));
	physicsEngine->collisionShapes.push_back(colShape);

	btTrans.setIdentity();
	btTrans.setFromOpenGLMatrix(glm::value_ptr(transform->pose));

	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);

	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(btTrans);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, convex, localInertia);
	//rbInfo.m_restitution = 1.0;
	rigidBody = new btRigidBody(rbInfo);

	isRigidBody = true;

	physicsEngine->dynamicsWorld->addRigidBody(rigidBody);

}

void BtConvexHullShapeBlender::solve(PhysicsEngine* physicsEngine)
{
	if (rigidBody && rigidBody->getMotionState())
		rigidBody->getMotionState()->getWorldTransform(btTrans);

	glm::mat4 openGLmatrix;
	btTrans.getOpenGLMatrix(glm::value_ptr(openGLmatrix));
	//transform->position(glm::vec3(btTrans.getOrigin().getX(), btTrans.getOrigin().getY(), btTrans.getOrigin().getZ()));
	transform->worldMatrix = openGLmatrix;
	transform->Update();

	//printf("world pos object %d = %f,%f,%f\n", 1, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
}

void BtConvexHullShapeBlender::render()
{
	Model::render();
	cubeMesh->Render();
}

BtConvexHullShapeBlender::~BtConvexHullShapeBlender()
{

}
