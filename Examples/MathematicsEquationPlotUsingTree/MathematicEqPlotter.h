#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Line.h"
#include "Plane.h"
#include "Cube.h"
#include "Sphere.h"
#include "BtConvexHullShapeBlender.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>
#include "PointForEq.h"
#include "PhysicsEngine.h"
#include "GLDebugDrawer.h"

#include "Input.h"

class MathematicEqPlotter
{
public:
	Texture* floorTexture[5];
	Texture* cubeTexture;

	Shader* floorShader;
	Shader* cubeShader;

	DirectionalLight* dlight;
	Line* l;
	SpotLight* slight;
	BtConvexHullShapeBlender* obj1;
	Material* floorMaterial;
	Material* cubeMaterial;
	vector< BtConvexHullShapeBlender*> v;
	vector< Cube*> c;
	//BtConvexHullShapeBlender* temp;
	Cube* cube, * temp;
	void Pillar(float,float,float);
	void PrintIt(PointForEq p);
	Plane* planePhy;
	PointForEq Copy(PointForEq p);
	Cube* cubePhy;
	Plane* sphere;
	vector<PointForEq> Eq;
	void computeHalfVertex(const float v1[3], const float v2[3], float newV[3]);
	void f1(int recurLimit,PointForEq p);
	PhysicsEngine* phyEng;
	GLDebugDrawer* debugDraw;
	float e = 2.718f;
	Input* input;

	Camera* camera;
	int Power(int x, int y);
	MathematicEqPlotter( int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input );
	void Update( float deltaTime );

	void Render();

};