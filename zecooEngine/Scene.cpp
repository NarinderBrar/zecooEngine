#include "Scene.h"

Scene::Scene(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine)
{
	phyEng = physicsEngine;

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT);
	camera->SetPerspectiveProjectionMatrix(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f);
	glm::vec3 camPos = glm::vec3(0.0, 300.0, 800.0);
	glm::vec3 camView = glm::vec3(0.0, 3.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
	camera->Set(camPos, camView, camUp);

	shader = new Shader("resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs");
	shader2 = new Shader("resources\\shader\\basicTextureLight.vs", "resources\\shader\\basicTextureLight.fs");

	dlight = new DirectionalLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, -1.0, 0.2));
	dlight->diffuse = glm::vec3(1.0, 1.0, 1.0);
	dlight->ambient = glm::vec3(0.5, 0.5, 0.5);

	glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	material = new Material(shader, color);
	material->linkLight(dlight);
	material->linkCamera(camera);

	plane = new Plane(material, NULL);
	plane->transform->scale(glm::vec3(5.0f, 5.0f, 5.0f));

	cube = new Cube(material, NULL);
	cube->transform->position(glm::vec3(0.0f, 3.0f, 0.0f));
	cube->transform->scale(glm::vec3(5.0f, 0.2f, 2.0f));
	cubes[0] = cube;

	color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	material2 = new Material(shader2, color);
	material2->linkLight(dlight);
	material2->linkCamera(camera);

	cube = new Cube(material2, NULL);
	cube->transform->translate(glm::vec3(0.0f, 2.0f, 0.0f));
	cube->transform->rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	cubes[1] = cube;
}

void Scene::Update(float deltaTime)
{
	camera->RotateViewPoint(800, glfwGetTime());
	projection = camera->GetPerspectiveProjectionMatrix();
}

void Scene::Render()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	plane->render();

	//cube->render();

	for (size_t i = 0; i < 2; i++)
	{
		cubes[i]->render();
	}
}