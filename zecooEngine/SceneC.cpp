#include "SceneC.h"

SceneC::SceneC(int SCR_WIDTH, int SCR_HEIGHT, PhysicsEngine* physicsEngine, Input* _input )
{
	phyEng = physicsEngine;

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	texture = new Texture();
	texture->Load("resources\\textures\\container.jpg");
	texture->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	texture->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	texture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	texture1 = new Texture();
	texture1->Load("resources\\textures\\marble.jpg");
	texture1->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	texture1->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture1->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	texture1->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT);
	camera->SetPerspectiveProjectionMatrix(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 5000.0f);
	glm::vec3 camPos = glm::vec3(0.0, 300.0, 800.0);
	glm::vec3 camView = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
	camera->Set(camPos, camView, camUp);

	std::cout << glm::to_string(camera->GetPosition()) << std::endl;
	std::cout << glm::to_string(camera->GetView()) << std::endl;

	shader = new Shader("resources\\shader\\6.multiple_lights.vs", "resources\\shader\\6.multiple_lights.fs");
	shaderL = new Shader("resources\\shader\\6.light_cube.vs", "resources\\shader\\6.light_cube.fs");
	shaderT = new Shader("resources\\shader\\4.1.texture.vs", "resources\\shader\\4.1.texture.fs");
	testShader = new Shader("resources\\shader\\testShader.vs", "resources\\shader\\testShader.fs");

	shader->use();
	shader->setInt("pointLightCount", 4);

	dlight = new DirectionalLight(pointLightPositions[0], glm::vec3(0.2, -1.0, 0.2));
	dlight->diffuse = glm::vec3(1.0, 1.0, 1.0);
	dlight->ambient = glm::vec3(0.1, 0.1, 0.1);

	plight0 = new PointLight(pointLightPositions[1], glm::vec3(0.2, -1.0, 0.2));
	plight1 = new PointLight(pointLightPositions[2], glm::vec3(0.2, -1.0, 0.2));
	plight2 = new PointLight(pointLightPositions[3], glm::vec3(0.2, -1.0, 0.2));
	plight3 = new PointLight(pointLightPositions[4], glm::vec3(0.2, -1.0, 0.2));

	slight = new SpotLight(pointLightPositions[5], glm::vec3(0.2, -1.0, 0.2));

	glm::vec4 color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	material = new Material(shader, color);

	material->linkLight(dlight);
	material->linkLight(plight0);
	material->linkLight(plight1);
	material->linkLight(plight2);
	material->linkLight(plight3);
	material->linkLight(slight);
	material->linkCamera(camera);

	color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	materialLight = new Material(shaderL, color);
	materialLight->linkCamera(camera);

	testMaterial = new Material(testShader, color);
	testMaterial->linkLight(dlight);
	testMaterial->linkLight(plight0);
	testMaterial->linkLight(plight1);
	testMaterial->linkLight(plight2);
	testMaterial->linkLight(plight3);
	testMaterial->linkLight(slight);
	testMaterial->linkCamera(camera);

	plane = new Plane(material, texture1);
	plane->transform->scale(glm::vec3(5.0f, 5.0f, 5.0f));

	triangle1 = new Triangle(material, texture1);
	triangle1->transform->translate(glm::vec3(-5.0f, 2.0f, 0.0f));

	cube = new Cube(material, texture1);
	cube->transform->translate(glm::vec3(0.5f, 0.75f, 0.0f));
	cube->SetRigidbody(phyEng);

	cubeLight = new Cube(materialLight, texture1);

	testModel = new TestModel(testMaterial, texture1);
	testModel->transform->translate(glm::vec3(0.0f, 1.0f, 1.0f));
}

void SceneC::Update(float deltaTime)
{
	camera->RotateViewPoint(800, glfwGetTime());
	projection = camera->GetPerspectiveProjectionMatrix();

	cube->solve(phyEng);

	float angle = 0.1f * deltaTime;
	triangle1->transform->rotate(glm::degrees(angle), glm::vec3(0.0f, 1.0f, 0.0f));

	testModel->transform->rotate(glm::degrees(angle), glm::vec3(0.0f, 1.0f, 0.0f));
}

void SceneC::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	plane->render();
	triangle1->render();
	cube->render();
	testModel->render();

	for (size_t i = 0; i < 4; i++)
	{
		cubeLight->transform->pose = glm::mat4(1.0f);
		cubeLight->transform->translate(pointLightPositions[i]);
		cubeLight->transform->scale(glm::vec3(0.1f, 0.1f, 0.1f));
		cubeLight->render();
	}
}