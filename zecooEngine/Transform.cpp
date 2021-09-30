#include "Transform.h"

Transform::Transform()
{
	worldMatrix = glm::mat4(1.0f);
}

void Transform::position(glm::vec3 vec)
{
	worldMatrix = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		vec.x, vec.y, vec.z, 1.0);
}

void Transform::setParent(Transform* parent)
{
	worldMatrix = parent->worldMatrix;
}

void Transform::translate(glm::vec3 vec)
{
	worldMatrix = glm::translate(worldMatrix, vec);
	Transformation();
}

void Transform::rotate(float angle, glm::vec3 axis)
{
	worldMatrix = glm::rotate(worldMatrix, glm::degrees(angle), axis);
	Transformation();
}

void Transform::scale(glm::vec3 vec)
{
	worldMatrix = glm::scale(worldMatrix, vec);
	Transformation();
}

void Transform::localTranslate(glm::vec3 vec)
{
	localTranslationMatrix = glm::translate(localTranslationMatrix, vec);
	Transformation();
}

void Transform::localRotate(float angle, glm::vec3 axis)
{
	localRotationMatrix = glm::rotate(localRotationMatrix, glm::degrees(angle), axis);
	Transformation();
}

void Transform::localScale(glm::vec3 vec)
{
	localScaleMatrix = glm::scale(localScaleMatrix, vec);
	Transformation();
}

void Transform::Transformation()
{
	pose = worldMatrix * localTranslationMatrix * localRotationMatrix  *localScaleMatrix;
}

Transform::~Transform()
{

}