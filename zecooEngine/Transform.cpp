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
	parentMatrix = parent->pose;
	Transformation();
}

void Transform::translate(glm::vec3 vec)
{
	localTranslationMatrix = glm::translate(localTranslationMatrix, vec);
	Transformation();
}

void Transform::rotate(float angle, glm::vec3 axis)
{
	localRotationMatrix = glm::rotate(localRotationMatrix, glm::degrees(angle), axis);
	Transformation();
}

void Transform::scale(glm::vec3 vec)
{
	localScaleMatrix = glm::scale(localScaleMatrix, vec);
	Transformation();
}

void Transform::worldTranslate(glm::vec3 vec)
{
	glm::mat4 I = glm::mat4(1.0);

	I = glm::translate(I, vec);
	worldMatrix = I * worldMatrix;

	Transformation();
}

void Transform::worldRotate(float angle, glm::vec3 axis)
{
	glm::mat4 I = glm::mat4(1.0);

	I = glm::rotate(I, glm::degrees(angle), axis);
	worldMatrix = I * worldMatrix;

	Transformation();
}

void Transform::worldScale(glm::vec3 vec)
{
	glm::mat4 I = glm::mat4(1.0);

	I = glm::scale(I, vec);
	worldMatrix = I * worldMatrix;

	Transformation();
}

void Transform::Transformation()
{
	//world translate ignore local rotation
	pose = parentMatrix  * worldMatrix * localTranslationMatrix * localRotationMatrix * localScaleMatrix;

	//local forward movement
	//world rotate and after that local translate
}

Transform::~Transform()
{

}