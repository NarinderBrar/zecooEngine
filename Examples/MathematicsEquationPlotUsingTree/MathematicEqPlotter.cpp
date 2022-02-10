#include "MathematicEqPlotter.h"

MathematicEqPlotter::MathematicEqPlotter( int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input )
{
	// configure global opengl state
	glEnable( GL_DEPTH_TEST );

	phyEng = physicsEngine;
	input = _input;

	floorTexture[0] = new Texture();
	floorTexture[0]->Load( "resources\\textures\\color1.png" );
	floorTexture[0]->SetSamplerObjectParameter( GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	floorTexture[0]->SetSamplerObjectParameter( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	floorTexture[0]->SetSamplerObjectParameter( GL_TEXTURE_WRAP_S, GL_REPEAT );
	floorTexture[0]->SetSamplerObjectParameter( GL_TEXTURE_WRAP_T, GL_REPEAT );

	floorTexture[1] = new Texture();
	floorTexture[1]->Load("resources\\textures\\color2.png");
	floorTexture[1]->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	floorTexture[1]->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	floorTexture[1]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	floorTexture[1]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	floorTexture[2] = new Texture();
	floorTexture[2]->Load("resources\\textures\\color3.png");
	floorTexture[2]->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	floorTexture[2]->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	floorTexture[2]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	floorTexture[2]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	floorTexture[3] = new Texture();
	floorTexture[3]->Load("resources\\textures\\color4.png");
	floorTexture[3]->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	floorTexture[3]->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	floorTexture[3]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	floorTexture[3]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	floorTexture[4] = new Texture();
	floorTexture[4]->Load("resources\\textures\\color5.png");
	floorTexture[4]->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	floorTexture[4]->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	floorTexture[4]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	floorTexture[4]->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	cubeTexture = new Texture();
	cubeTexture->Load( "resources\\textures\\cylinder.png" );
	cubeTexture->SetSamplerObjectParameter( GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	cubeTexture->SetSamplerObjectParameter( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	cubeTexture->SetSamplerObjectParameter( GL_TEXTURE_WRAP_S, GL_REPEAT );
	cubeTexture->SetSamplerObjectParameter( GL_TEXTURE_WRAP_T, GL_REPEAT );

	camera = new Camera( SCR_WIDTH, SCR_HEIGHT );
	camera->SetPerspectiveProjectionMatrix( glm::radians( 45.0f ), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f );
	glm::vec3 camPos = glm::vec3( 0.0, 300.0, 400.0 );
	glm::vec3 camView = glm::vec3( 0.0, 3.0, 0.0 );
	glm::vec3 camUp = glm::vec3( 0.0, 1.0, 0.0 );
	camera->Set( camPos, camView, camUp );

	cubeShader = new Shader( "resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs" );

	debugDraw = new GLDebugDrawer( camera );
	phyEng->dynamicsWorld->setDebugDrawer( debugDraw );

	dlight = new DirectionalLight(glm::vec3(0.0, 10.0, 30.0), glm::vec3(0.0, -3.0, -10.0));
	dlight->diffuse = glm::vec3(0.75, 0.14,0.10);
	dlight->ambient = glm::vec3(0.21, 0.11, 0.14);

	glm::vec4 color = glm::vec4(0.0f, 0.5f, 0.5f, 1.0f);
	floorMaterial = new Material(cubeShader, color);
	floorMaterial->linkLight(dlight);
	floorMaterial->linkCamera(camera);

	/*glm::vec4 color = glm::vec4( 0.5f, 0.5f, 0.5f, 0.5f );
	floorMaterial = new Material( cubeShader, color );
	floorMaterial->linkLight( dlight );
	floorMaterial->linkCamera( camera );*/

	cubeMaterial = new Material( cubeShader, color );
	cubeMaterial->linkLight( dlight );
	cubeMaterial->linkCamera( camera );
	float mass = 1.0f;
	int pcount = 0, icount = 0, uvcount = 0;
	//vector<float> v;
	string myText;
	ifstream MyReadFile("dataset.csv"); //reader
	int i = 0, j = -1, k = 0;
	/*while (getline(MyReadFile, myText))
	{
		k++;
		if (k == 50)
		{
			break;
		}
		if (myText.rfind("", 0) == 0) {
			i++; pcount++;
			string s = myText.substr(0, myText.length());
			const int n = s.length();
			char* char_array = new char[n + 1];
			strcpy(char_array, s.c_str());
			//p->x = new float(); p->y = new float(); p->z = new float();
			float a, b, c,d,e;
			sscanf(char_array, "%f,%f,%f,%f,%f", &a, &b, &c,&d,&e);
			float value = (int)(a * 100 + .5);
			a = (float)value / 100; //if (a < 0.0f) a = a * -1.0f;
			value = (int)(b * 100 + .5); 
			b = (float)value / 100;  //if (b < 0.0f) b = b * -1.0f;
			value = (int)(c * 100 + .5);
			c = (float)value / 100;	if (c < 0.0f) c = c * -1.0f;
			value = (int)(d * 100 + .5);
			d = (float)value / 100;	if (d < 0.0f) d = d * -1.0f;
			int x = -10, y = 10;
			float m = (Power(x,3) + 4 * Power(y, 2)) * sqrt(abs(sin(Power(x, 2) + Power(y, 2))));
			cout << m/50.0f << endl;
		//	cout <<" "<< a << "  " << b << "  " << c << "  " << d << endl;
		//	Pillar(a*1.0f,b * 1.0f,c * 1.0f);
		}
	}*/

	/*cubePhy = new Cube( cubeMaterial, cubeTexture );
	cubePhy->transform->translate( glm::vec3( 0.0f, 0.0f, 0.0f ) );
	cubePhy->mass = 1.0;
	cubePhy->SetRigidbody( phyEng );*/

	/*planePhy = new Plane( floorMaterial, floorTexture );
	planePhy->transform->scale( glm::vec3( 5.0f, 0.0f, 5.0f ) );
	//planePhy->transform->rotate( 0.0, glm::vec3( 0.0f, 0.0f, 1.0f ) );
	planePhy->mass = 0.0;
	planePhy->SetRigidbody( phyEng );*/

	/*sphere = new Plane( floorMaterial, NULL );
	sphere->transform->scale( glm::vec3( 20.0f,0.1f,20.0f ) );
	sphere->transform->translate( glm::vec3( 0.0f, -1.0f, 0.0f ) );
	sphere->mass = 0.0;
	sphere->SetRigidbody( phyEng );*/

	/*obj1 = new BtConvexHullShapeBlender(floorMaterial, floorTexture, "a.obj", "a.obj");
	obj1->transform->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	obj1->mass = 0.0;
	//obj1->transform->rotate(3.14 / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	obj1->SetRigidbody(phyEng);*/
	//v.push_back(obj1);

/*	obj1 = new BtConvexHullShapeBlender(floorMaterial, floorTexture, "a.obj", "a.obj");
	obj1->transform->translate(glm::vec3(0.0f, 15.0f, 2.0f));
	obj1->mass = 1.0;
	obj1->SetRigidbody(phyEng);
	v.push_back(obj1);*/
	//l = new Line(floorMaterial);
	//l->setPoints(glm::vec3(0,0,0), glm::vec3(1, 0, 0));
	//cout << Power(5, 2);
	/*PointForEq p;
	int x = -10, y = 10;
	float m = (Power(x, 3) + 4 * Power(y, 2)) * sqrt(abs(sin(Power(x, 2) + Power(y, 2))));
	p.x1 = x / 10;  p.y1 = y / 10; p.z1 = m/1500.0f;
	x = -10, y = -10;
	m = (Power(x, 3) + 4 * Power(y, 2)) * sqrt(abs(sin(Power(x, 2) + Power(y, 2))));
	p.x2 = x / 10;  p.y2 = y / 10; p.z2 = m/1500.0f;
	x = 10, y = -10;
	m = (Power(x, 3) + 4 * Power(y, 2)) * sqrt(abs(sin(Power(x, 2) + Power(y, 2))));
	p.x3 = x/10;  p.y3 = y/10; p.z3 = m / 1500.0f;
	x = 10, y = 10;
	m = (Power(x, 3) + 4 * Power(y, 2)) * sqrt(abs(sin(Power(x, 2) + Power(y, 2))));
	p.x4 = x/10;  p.y4 = y/10; p.z4 = m / 1500.0f;
	//cout << " " << p.z1 << "  " << p.z2 << "  " << p.z3 << "  " << p.z4 << endl;*/
	PointForEq p;
	float multi = 3.0f;
	p.x1 = -1*multi; p.y1 = -1 * multi; p.z1 = 0;
	p.x2 = 1 * multi; p.y2 = -1 * multi; p.z2 = 0;
	p.x3 = 1 * multi; p.y3 = 1 * multi; p.z3 = 0;
	p.x4 = -1 * multi; p.y4 = 1 * multi; p.z4 = 0;
	f1(4, p);
	cout << "  "<<Eq.size()<<endl;
	/*for (PointForEq f : Eq)
	{
		PointForEq temp = Copy(f);
		int h=(rand() % 5);
		
		obj1 = new BtConvexHullShapeBlender(floorMaterial,floorTexture[h], "cube.obj", &(temp));
		obj1->transform->translate(glm::vec3(0.0f, 1.0f, 0.0f));
		obj1->mass = 0.0;
		//obj1->transform->rotate(3.14 / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		obj1->SetRigidbody(phyEng);
		v.push_back(obj1);
	}*/
	for (int ew = 0;ew < Eq.size();ew++)
	{
		/*if (ew != 1)
		{
			continue;
		}*/
		PointForEq temp = Copy(Eq[ew]);
		int h = (rand() % 5);
		obj1 = new BtConvexHullShapeBlender(floorMaterial, floorTexture[h], "cube.obj", &(temp));
		obj1->transform->translate(glm::vec3(0.0f, 1.0f, 0.0f));
		obj1->mass = 0.0;
		//obj1->transform->rotate(3.14 / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		obj1->SetRigidbody(phyEng);
		v.push_back(obj1);
	}
	
}
void MathematicEqPlotter::PrintIt(PointForEq p)
{
	cout << " " << p.x1 << "  " << p.y1 << "  " << p.z1 << endl;
	cout << " " << p.x2 << "  " << p.y2 << "  " << p.z2 << endl;
	cout << " " << p.x3 << "  " << p.y3 << "  " << p.z3 << endl;
	cout << " " << p.x4 << "  " << p.y4 << "  " << p.z4 << endl;
	cout << endl;
}
PointForEq MathematicEqPlotter::Copy(PointForEq p)
{
	PointForEq x;
	x.x1 = p.x1; x.x2 = p.x2; x.x3 = p.x3; x.x4 = p.x4;
	x.y1 = p.y1; x.y2 = p.y2; x.y3 = p.y3; x.y4 = p.y4;
	x.z1 = p.z1; x.z2 = p.z2; x.z3 = p.z3; x.z4 = p.z4;
	return x;
}
int MathematicEqPlotter::Power(int base, int powerRaised)
{
	if (powerRaised != 0)
		return (base *Power(base, powerRaised - 1));
	else
		return 1;
}
void MathematicEqPlotter::computeHalfVertex(const float v1[3], const float v2[3], float newV[3])
{
	newV[0] = v1[0] + v2[0];    // x
	newV[1] = v1[1] + v2[1];    // y
	newV[2] = v1[2] + v2[2];    // z
	float scale = 1.0 / sqrtf(newV[0] * newV[0] + newV[1] * newV[1] + newV[2] * newV[2]);
	newV[0] *= scale;
	newV[1] *= scale;
	newV[2] *= scale;
}
void MathematicEqPlotter::f1(int recurLimit,PointForEq p)
{
	if (recurLimit == 1)
	{
		PointForEq pf;
		//int x = -10, y = 10; 7xy/e^(x^2 + y^2)
		float m = 4/e*(Power(p.x1, 2) +  Power(p.y1, 2));
		
		pf.x1 = p.x1 / 1;  pf.y1 = p.y1 / 1; pf.z1 = m ;
		
		//x = -10, y = -10;
		m = 4/ e * (Power(p.x2, 2) + Power(p.y2, 2));
		pf.x2 = p.x2 / 1;  pf.y2 = p.y2 / 1; pf.z2 = m ;
		cout << "m  " << pf.z2 << endl;
		//x = 10, y = -10;
		m = 4/ e * (Power(p.x3, 2) + Power(p.y3, 2));
		pf.x3 = p.x3 / 1;  pf.y3 = p.y3 / 1; pf.z3 = m ;
		//x = 10, y = 10;
		m = 4 / e * (Power(p.x4, 2) + Power(p.y4, 2));
		pf.x4 = p.x4 / 1;  pf.y4= p.y4 / 1; pf.z4 = m ;
		PrintIt(pf);
		Eq.push_back(pf);
		//cout << " " << p.z1 << "  " << p.z2 << "  " << p.z3 << "  " << p.z4 << endl;
	}
	else
	{
		for (int i = 0;i < 1;i++)
		{
		//	p.x1 p.y1 p.z1  p.x2 p.y2 p.z2
			PointForEq p2;
			p2.x1 = p.x1+p.x2; p2.y1 =p.y1; p2.z1 = -p.z2 + p.z1;
			p2.x2 = p.x2 ; p2.y2 = p.y2+p.y3; p2.z2 = p.z3 - p.z2;
			p2.x3 = p.x3+p.x4; p2.y3 = p.y3; p2.z3 = p.z3 - p.z4;
			p2.x4 = p.x4; p2.y4 = p.y4+p.y1; p2.z4 = p.z4 - p.z1;
			float midx = p2.x1, midy = p2.y3 + p2.y1, midz= p2.z3 + p2.z1;
			/*p2.x1 = p.x1; p2.y1 = p.y1; p2.z1 = p.z1;
			p2.x2 = (p.x2 - p.x1); p2.y2 = (p.y2 - p.y1); p2.z2 = (p.z2 - p.z1);
			//mid of 3,4
			float a, b, c;
			a= (p.x3 - p.x4); b = (p.y3 - p.y4); c = (p.z3 - p.z4);
			p2.x3 = (a - p2.x2); p2.y3 = (b - p2.y2); p2.z3 = (c - p2.z2);
			p2.x4 = (p.x4 - p.x1); p2.y4 = (p.y4 - p.y1); p2.z4 = (p.z4 - p.z1);*/
			PointForEq a1; a1.x1 = p.x1; a1.y1 = p.y1; a1.z1 = p.z1;
			a1.x2 = p2.x1; a1.y2 = p2.y1; a1.z2 = p2.z1;
			a1.x3 = midx; a1.y3 =midy; a1.z3 = midz;
			a1.x4 =p2.x4; a1.y4 = p2.y4; a1.z4 = p2.z4;
			//PrintIt(a1);
			f1(recurLimit - 1, a1);
			PointForEq a2; a2.x2 = p.x2; a2.y2 = p.y2; a2.z2 = p.z2;
			a2.x1 = p2.x1; a2.y1 = p2.y1; a2.z1 = p2.z1;
			a2.x3 = p2.x2; a2.y3 = p2.y2; a2.z3 = p2.z2;
			a2.x4 = midx; a2.y4 = midy; a2.z4 = midz;
			//PrintIt(a2);
			f1(recurLimit - 1, a2);
			PointForEq a3; a3.x2 = p2.x2; a3.y2 = p2.y2; a3.z2 = p2.z2;
			a3.x1 = midx; a3.y1 = midy; a3.z1 = midz;
			a3.x3 = p.x3; a3.y3 = p.y3; a3.z3 = p.z3;
			a3.x4 = p2.x3; a3.y4 = p2.y3; a3.z4 = p2.z3;
			//PrintIt(a3);
			f1(recurLimit - 1, a3);
			PointForEq a4; a4.x2 =midx; a4.y2 =midy; a4.z2 =midz;
			a4.x1 = p2.x4; a4.y1 = p2.y4; a4.z1 = p2.z4;
			a4.x3 = p2.x3; a4.y3 = p2.y3; a4.z3 = p2.z3;
			a4.x4 = p.x4; a4.y4 = p.y4; a4.z4 = p.z4;
			//PrintIt(a4);
			f1(recurLimit - 1, a4);
		}
	}
}
void MathematicEqPlotter::Pillar(float x,float y,float z)
{
	/*for (int i = 0;i < 0;i++)
	{
		string stg = "S" + to_string(i); stg += ".obj";
		temp = new BtConvexHullShapeBlender(floorMaterial, floorTexture, stg, stg);
		//temp->transform->translate(glm::vec3(-22.0f, 2.0f, 0.0f));
		temp->transform->translate(glm::vec3(0.0f, 0.0f, 0.0f));
		//	planePhy1->transform->scale(glm::vec3(10.0f, 1.0f, 10.0f));
		//temp->transform->rotate(3.14f, glm::vec3(1.0f, 0.0f, 0.0f));
		temp->mass = 0.0;
		temp->SetRigidbody(phyEng);
		temp->name = "p";
		temp->rigidBody->setUserPointer(temp);
		v.push_back(temp);

	}
	for (int i = 0;i <1;i++)
	{*/
		//if (i == 1|| i == 1) continue;
		//string stg = "A" + to_string(i); stg += ".obj";
		string stg = "A0.obj";
		//temp = new BtConvexHullShapeBlender(floorMaterial, cubeTexture, stg, stg);
		temp = new Cube(floorMaterial, cubeTexture);
		//temp->transform->translate(glm::vec3(-22.0f, 2.0f, 0.0f));
		temp->transform->translate(glm::vec3(x-0.0f, 1.25f,z-0.0f));
		temp->transform->scale(glm::vec3(0.2f, y*0.2f, 0.2f));
		temp->transform->rotate(3.14f, glm::vec3(1.0f, 0.0f, 0.0f));
		temp->mass = 1.0;
		temp->SetRigidbody(phyEng);
		temp->name = "p";
		temp->rigidBody->setUserPointer(temp);
		c.push_back(temp);

	//}
}
void MathematicEqPlotter::Update( float deltaTime )
{
	if( input->getPressedKey() == "Up" )
	{
		//sphere->rigidBody->applyForce( btVector3( 5, 0, 0 ), btVector3(0,0.5,0) );
	}
	for (BtConvexHullShapeBlender* x : v)
	{
		x->solve(phyEng);
	}
	for (Cube* x : c)
	{
		x->solve(phyEng);
	}
	//cubePhy->solve( phyEng );
	//planePhy->solve( phyEng );
	//sphere->solve( phyEng );
	//camera->FPS();
	//camera->Strafe(1000.0);

	camera->RotateViewPoint( 1800, glfwGetTime() );
}

void MathematicEqPlotter::Render()
{
	glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//planePhy->render();
	//cubePhy->render();
	//sphere->render();
	//l->render();
	for (BtConvexHullShapeBlender* x : v)
	{
		x->render();
	}
	for (Cube* x : c)
	{
		x->render();
	}
	//phyEng->dynamicsWorld->debugDrawWorld();
	phyEng->dynamicsWorld->getDebugDrawer()->setDebugMode( btIDebugDraw::DebugDrawModes::DBG_DrawWireframe );
	phyEng->dynamicsWorld->debugDrawWorld();
}