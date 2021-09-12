#include "Debugger.h"

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

void Debugger::createLine(glm::vec3 pointA, glm::vec3 pointB, Camera* camera)
{
	shader = new Shader("resources\\shader\\unlit.vs", "resources\\shader\\unlit.fs");
	
	glm::vec4 color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	material = new Material(shader, color);
	material->linkCamera(camera);

	line = new Line(material);

	line->setPoints(pointA, pointB);
}

void Debugger::createRay(glm::vec3 origin, glm::vec3 direction, Camera* camera)
{
	shader = new Shader("resources\\shader\\unlit.vs", "resources\\shader\\unlit.fs");

	glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	material = new Material(shader, color);
	material->linkCamera(camera);

	ray = new Ray(material);

	ray->set(origin, direction);
}

void Debugger::drawLine()
{
	line->render();
}

void Debugger::drawRay()
{
	ray->render();
}
