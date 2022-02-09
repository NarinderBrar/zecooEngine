#include "Scene.h"

Scene::Scene(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine)
{
	phyEng = physicsEngine;

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	walls = new Texture();
	walls->Load("resources\\textures\\wood.png");
	walls->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	walls->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	walls->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	walls->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	walls1 = new Texture();
	walls1->Load("resources\\textures\\wood1.png");
	walls1->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	walls1->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	walls1->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	walls1->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	floor = new Texture();
	floor->Load("resources\\textures\\floor.jpg");
	floor->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	floor->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	floor->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	floor->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	windows = new Texture();
	windows->Load("resources\\textures\\window1.jpg");
	windows->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	windows->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	windows->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	windows->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	tel = new Texture();
	tel->Load("resources\\textures\\tele.jpg");
	tel->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	tel->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	tel->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tel->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	tab = new Texture();
	tab->Load("resources\\textures\\table.jpg");
	tab->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	tab->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	tab->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tab->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	marble = new Texture();
	marble->Load("resources\\textures\\table1.jpg");
	marble->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	marble->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	marble->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	marble->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	sof = new Texture();
	sof->Load("resources\\textures\\sofa.jpg");
	sof->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	sof->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	sof->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	sof->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	pic = new Texture();
	pic->Load("resources\\textures\\pic.jpg");
	pic->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	pic->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	pic->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	pic->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT);
	camera->SetPerspectiveProjectionMatrix(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 2.0f, 5000.0f);
	glm::vec3 camPos = glm::vec3(0.0, 300.0, -1500.0);
	glm::vec3 camView = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
	camera->Set(camPos, camView, camUp);

	shader = new Shader("resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs");

	grid = new Grid(camera);

	dlight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, -1.0, 0.2));
	dlight->diffuse = glm::vec3(1.0, 1.0, 1.0);
	dlight->ambient = glm::vec3(0.5, 0.5, 0.5);

	glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	material = new Material(shader, color);
	material->linkLight(dlight);
	material->linkCamera(camera);

	/*
	plane = new Plane(material, floorTexture);

	debugger = new Debugger(camera);

	cylinder = new Cylinder(material, floorTexture);

	triangle = new Triangle(material, NULL);

	*/


	/*---------------------------------------------------*/

	/*	Floor And Wall	*/
	cube = new Cube(material, floor);
	cube->transform->position(glm::vec3(0, 0, 0));
	cube->transform->scale(glm::vec3(12.5, 0.25, 12.5));

	cube1 = new Cube(material, walls);
	cube1->transform->position(glm::vec3(0, 2.35, 6.5));
	cube1->transform->scale(glm::vec3(12.5, 5.0, 0.5));

	cube2 = new Cube(material, walls1);
	cube2->transform->position(glm::vec3(6.5, 2.35, 0.0));
	cube2->transform->scale(glm::vec3(0.5, 5.0, 12.5));

	/*	Windows	*/
	cube3 = new Cube(material, windows);
	cube3->transform->position(glm::vec3(6.5, 2.35, 0.0));
	cube3->transform->scale(glm::vec3(0.65, 2.0, 3.0));

	/*	Interior */
	tele = new Cube(material, tel);
	tele->transform->position(glm::vec3(0, 2.35, 6.2));
	tele->transform->scale(glm::vec3(4.0, 2.5, 0));

	table = new Cube(material, tab);
	table->transform->position(glm::vec3(0, 0.5, 6.0));
	table->transform->scale(glm::vec3(2.5, 1.0, 0.5));

	cube4 = new Cube(material, marble);
	cube4->transform->position(glm::vec3(0, 1.25, 1.5));
	cube4->transform->scale(glm::vec3(3.5, 0.2, 2.0));

	cube6 = new Cube(material, marble);
	cube6->transform->position(glm::vec3(0, 0.25, 1.5));
	cube6->transform->scale(glm::vec3(3.5, 0.2, 2.0));

	cube5 = new Cube(material, marble);
	cube5->transform->position(glm::vec3(0, 1.0, 1.5));
	cube5->transform->scale(glm::vec3(0.5, 1.5, 0.5));

	sofa = new Cube(material, sof);
	sofa->transform->position(glm::vec3(0, 0.5, -3.0));
	sofa->transform->scale(glm::vec3(5.0, 1.25, 2.0));

	sofa2 = new Cube(material, sof);
	sofa2->transform->position(glm::vec3(0, 1.55, -3.85));
	sofa2->transform->scale(glm::vec3(5.0, 1.4, 0.3));

	frame = new Cube(material, pic);
	frame->transform->position(glm::vec3(6.25, 2.35, 3.5));
	frame->transform->scale(glm::vec3(0.1, 3.5, 2.5));
	/*---------------------------------------------------*/

	

	glm::mat4 pos = glm::mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 2.0, 1.0
	);

	glm::mat4 scale = glm::mat4(
		5.0, 0.0, 0.0, 0.0,
		0.0, 2.0, 0.0, 0.0,
		0.0, 0.0, 2.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);

	glm::mat4 iden = glm::mat4(1.0);

	glm::mat4 ans = iden * pos * scale;

	matrix = new Cube(material, NULL);
	matrix->transform->pose = ans;

}

void Scene::Update(float deltaTime)
{
	camera->RotateViewPoint(1000, glfwGetTime());
	projection = camera->GetPerspectiveProjectionMatrix();
}

void Scene::Render()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	debugger->draw();

    cylinder->render();

	triangle->render();

	*/

	//grid->Render();

	
	cube->render();
	cube1->render();
	cube2->render();
	cube3->render();
	cube4->render();
	cube5->render();
	cube6->render();
	tele->render();
	table->render();
	sofa->render();
	sofa2->render();
	frame->render();
	

	//matrix->render();
}