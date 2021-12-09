#include "Wave.h"
Wave::Wave(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input)
{
	phyEng = physicsEngine;

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT);
	camera->SetPerspectiveProjectionMatrix(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f);
	glm::vec3 camPos = glm::vec3(450.0, 350.0, 500.0);
	glm::vec3 camView = glm::vec3(0.0, 2.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
	camera->Set(camPos, camView, camUp);

	//std::cout << glm::to_string(camera->GetPosition()) << std::endl;
	//std::cout << glm::to_string(camera->GetView()) << std::endl;

	shader = new Shader("resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs");
	Ashader = new Shader("resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs");
	
	grid = new Grid(camera);

	dlight = new DirectionalLight(glm::vec3(0.0, 10.0, 30.0), glm::vec3(0.0, -3.0, -10.0));
	dlight->diffuse = glm::vec3(1.0, 1.0, 1.0);
	dlight->ambient = glm::vec3(0.1, 0.1, 0.1);

	glm::vec4 color = glm::vec4(0.0f, 0.5f, 0.5f, 1.0f);
	material = new Material(shader, color);
	material->linkLight(dlight);
	material->linkCamera(camera);

	glm::vec4 color1 = glm::vec4(0.0f, 0.5f, 0.5f, 1.0f);
	testMaterial = new Material(Ashader, color1);
	testMaterial->linkLight(dlight);
	testMaterial->linkCamera(camera);

	for(int i = 0;i < 15;i++)
	{
		glm::vec3 scale = glm::vec3(0.2f,0.2f,0.2f);
		for(int j=0;j<15;j++)
		{
			cubes[i][j] = new Cube(material, NULL);
			cubes[i][j]->transform->scale(scale);
		}
	}
}

void Wave::calcenter()
{
	float cx, cz, m = 0;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			//cx = cubes[i][j] * i;
			//cz = cubes[i][j] * j;
			//m += cubes[i][j];
		}
	}
//	int cmx, cmz = 0;
	//cmx = (int)cx / (int)m;
	//cmz = (int)cz / (int)m;
}


void Wave::sinwave(float deltaTime)
{
	float xComponent = 0.0f;
	float zComponent = 0.0f;

	angle += deltaTime;

	for (int i = 0; i < 15; i++)
	{
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
		pos.x = (i)*0.5 - 3.5;
		
		for (int j = 0; j < 15; j++) 
		{	
			xComponent = (8 - ( i + 1 ) ) ;
			zComponent = ( 8 - ( j + 1 ) ) ;
			pos.y = amplitude * sin(sqrt(xComponent*xComponent + zComponent*zComponent)+angle);
			
			pos.z = (j)*0.5 - 3.5;

			cubes[i][j]->transform->position(pos);
		}
	}
}

void Wave::Update(float deltaTime)
{
	camera->RotateViewPoint(700, glfwGetTime());
	projection = camera->GetPerspectiveProjectionMatrix();
	sinwave(deltaTime);
}

void Wave::Render()
{
	glClearColor(1.0f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i=0;i<15;i++)
	{
		for (int j = 0; j < 15; j++) 
		{
			cubes[i][j]->render();
		}
	}
	grid->Render();
}