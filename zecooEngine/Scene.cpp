#include "Scene.h"

Scene::Scene(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine)
{
	phyEng = physicsEngine;

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	floorTexture = new Texture();
	floorTexture->Load("resources\\textures\\uv.jpg");
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	floorTexture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT);
	camera->SetPerspectiveProjectionMatrix(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f);
	glm::vec3 camPos = glm::vec3(0.0, 300.0, -1500.0);
	glm::vec3 camView = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
	camera->Set(camPos, camView, camUp);

	shader = new Shader("resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs");

	dlight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, -1.0, 0.2));
	dlight->diffuse = glm::vec3(1.0, 1.0, 1.0);
	dlight->ambient = glm::vec3(0.5, 0.5, 0.5);

	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	material = new Material(shader, color);
	material->linkLight(dlight);
	material->linkCamera(camera);

	cube = new Cube(material, NULL);
	cube->transform->position(glm::vec3(2, 0.0, 2));
	//cube->transform->scale(glm::vec3(0.2, 0.2, 0.2));

	//cylinder = new Cylinder(material, floorTexture);
	grid = new Grid(camera);
}

void Scene::Update(float deltaTime)
{
	camera->RotateViewPoint(500, glfwGetTime());
	projection = camera->GetPerspectiveProjectionMatrix();
}

void Scene::Render()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cube->render();

	grid->Render();

	//cylinder->render();
}