#include "Ray.h"

Ray::Ray(Material* _material) : Model(_material, NULL)
{
	lineMesh = new LineMesh();
}

void Ray::set(glm::vec3 _origin, glm::vec3 _direction)
{
	glm::vec3 normalizeDirection = glm::normalize(_direction);

	double c = 10.0;
	normalizeDirection = glm::vec3(normalizeDirection.x * c, normalizeDirection.y * c, normalizeDirection.z * c);
	
	glm::vec3 pointB = _origin + normalizeDirection;
	lineMesh->setPoints(_origin, pointB);
}

void Ray::render()
{
	Model::render();

	lineMesh->Render();
}

Ray::~Ray()
{
}
