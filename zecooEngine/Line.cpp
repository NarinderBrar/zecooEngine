#include "Line.h"

Line::Line(Material* _material) : Model(_material, NULL)
{
	lineMesh = new LineMesh();
}

void Line::setPoints(glm::vec3 _pointA, glm::vec3 _pointB)
{
	lineMesh->setPoints(_pointA, _pointB);
}

void Line::render()
{
	Model::render();

	lineMesh->Render();
}

Line::~Line()
{
}
