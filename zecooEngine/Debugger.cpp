#include "Debugger.h"

Debugger::Debugger(Camera* _camera)
{
	camera = _camera;
}

void Debugger::printMsg(string txt)
{
	cout << "Msg:" << txt << endl;
}

void Debugger::printWarning(string txt)
{
	cout << "Warning:" << txt << endl;
}

void Debugger::printError(string txt)
{
	cout << "Error:" << txt << endl;
}

void Debugger::addLine(glm::vec3 pointA, glm::vec3 pointB, glm::vec4 color)
{
	shader = new Shader("resources\\shader\\unlit.vs", "resources\\shader\\unlit.fs");
	
	material = new Material(shader, color);
	material->linkCamera(camera);

	line = new Line(material);

	line->setPoints(pointA, pointB);

	lines.push_back(line);
}

void Debugger::addRay(glm::vec3 origin, glm::vec3 direction, glm::vec4 color)
{
	shader = new Shader("resources\\shader\\unlit.vs", "resources\\shader\\unlit.fs");

	material = new Material(shader, color);
	material->linkCamera(camera);

	ray = new Ray(material);

	ray->set(origin, direction);

	rays.push_back(ray);
}

void Debugger::addPoint(glm::vec3 position, glm::vec4 color)
{
	shader = new Shader("resources\\shader\\unlit.vs", "resources\\shader\\unlit.fs");

	material = new Material(shader, color);
	material->linkCamera(camera);

	cube = new Cube(material, NULL);
	cube->transform->position(position);
	cube->transform->scale(glm::vec3(0.1, 0.1, 0.1));
	
	models.push_back(cube);
}

void Debugger::draw()
{
	for (int i = 0; i < rays.size(); i++)
	{
		rays[i]->render();
	}
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i]->render();
	}
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->render();
	}
}
