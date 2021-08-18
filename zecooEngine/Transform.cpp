#include "Transform.h"

Transform::Transform()
{
	pose = glm::mat4(1.0f);
}

void Transform::position(glm::vec3 vec)
{
	pose = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		vec.x, vec.y, vec.z, 1.0);
}

void Transform::setParent(Transform* parent)
{
	pose = parent->pose;
}

void Transform::translate(glm::vec3 vec)
{
	pose = glm::translate(pose, vec);
}

void Transform::rotate(float angle, glm::vec3 axis)
{
	pose = glm::rotate(pose, glm::degrees(angle), axis);
}

void Transform::scale(glm::vec3 vec)
{
	pose = glm::scale(pose, vec);
}

Transform::~Transform()
{

}