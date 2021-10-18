#include "Transform.h"

Transform::Transform()
{
	worldMatrix = glm::mat4(1.0f);
// 00 01 02 03
// 10 11 12 13
// 20 21 22 23
// 30 31 32 33
}

void Transform::position(glm::vec3 vec)
{
	worldMatrix[3][0] = vec.x;
	worldMatrix[3][1] = vec.y;
	worldMatrix[3][2] = vec.z;
}

glm::vec3 Transform::getPosition()
{
	cubPos = glm::vec3( pose[3][0], pose[3][1], pose[3][2] );
	return cubPos;
}

void Transform::setParent(Transform* _parent)
{
	parent = _parent;
	Update();
}

void Transform::translate(glm::vec3 vec)
{
	worldMatrix = glm::translate( worldMatrix, vec);
	Update();
}

void Transform::rotate(float angle, glm::vec3 axis)
{
	worldMatrix = glm::rotate( worldMatrix, glm::degrees(angle), axis);
	Update();
}

void Transform::scale(glm::vec3 vec)
{
	worldMatrix = glm::scale( worldMatrix, vec);
	localScaleMatrix = glm::scale( localScaleMatrix, vec );
	Update();
}

void Transform::worldTranslate(glm::vec3 vec)
{
	glm::mat4 I = glm::mat4(1.0);

	I = glm::translate(I, vec);
	worldMatrix = I * worldMatrix;

	Update();
}

void Transform::worldRotate(float angle, glm::vec3 axis)
{
	glm::mat4 I = glm::mat4(1.0);

	I = glm::rotate(I, glm::degrees(angle), axis);
	worldMatrix = I * worldMatrix;

	Update();
}

void Transform::worldScale(glm::vec3 vec)
{
	glm::mat4 I = glm::mat4(1.0);

	I = glm::scale(I, vec);
	worldMatrix = I * worldMatrix;

	Update();
}
void Transform::resetParentScale( glm::vec3 vec )
{
	glm::mat4 I = glm::mat4( 1.0 );

	I = glm::scale( I, vec );

	if( parent != NULL )
			parentMatrix = ( parent->pose ) / I;
}

void Transform::MoveTowards( float dt, Transform* target, float speed, float offset )
{
	glm::vec3 pos = getPosition();
	glm::vec3 tPos = target->getPosition();

	//check distance between CubeHPos and newPos
	float a = glm::distance( tPos, getPosition() );

	//if distance is more than 1.0, move toward cube
	if( a > offset )
	{
		//calculate move direction, that will be normalized CubeHPos - newPos
		glm::vec3 dir = glm::normalize( tPos - pos );

		//simply translate cubeR with this direction, multiply dt and any other value for speed
		translate( dir * dt * speed );
	}
}

void Transform::Update()
{
	pose =  parentMatrix  * worldMatrix;
}

Transform::~Transform()
{

}