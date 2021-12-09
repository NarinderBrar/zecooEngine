#include "Transform.h"

Transform::Transform()
{
	worldMatrix = glm::mat4(1.0f);
// 00 01 02 03
// 10 11 12 13
// 20 21 22 23
// 30 31 32 33
	Update();
}

void Transform::position(glm::vec3 vec)
{
	worldMatrix[3][0] = vec.x;
	worldMatrix[3][1] = vec.y;
	worldMatrix[3][2] = vec.z;
	Update();
}

glm::vec3 Transform::getPosition()
{
	cubPos = glm::vec3( pose[3][0], pose[3][1], pose[3][2] );
	return cubPos;
}

glm::vec3 Transform::getlocalXDir()
{
	return glm::vec3( pose[0][0], pose[0][1], pose[0][2] );
}

glm::vec3 Transform::getlocalYDir()
{
	return glm::vec3( pose[1][0], pose[1][1], pose[1][2] );
}

glm::vec3 Transform::getlocalZDir()
{
	return glm::vec3( pose[2][0], pose[2][1], pose[2][2] );
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
	eulerAngle += axis * angle;
	worldMatrix = glm::rotate( worldMatrix, glm::degrees(angle), axis);
	Update();
}

void Transform::scale(glm::vec3 vec)
{
	worldMatrix = glm::scale( worldMatrix, vec);
	//localScaleMatrix = glm::scale( localScaleMatrix, vec );
	localScale = vec;
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

void Transform::lookAt( glm::vec3 eye, glm::vec3 at, glm::vec3 up )
{
	glm::vec3 zaxis = glm::normalize( at - eye );
	glm::vec3 xaxis = glm::normalize( glm::cross( zaxis, up ) );
	glm::vec3 yaxis = glm::cross( xaxis, zaxis );

	//negate( zaxis );

	glm::mat4 viewMatrix = glm::mat4{
	  xaxis.x, xaxis.y, xaxis.z, -glm::dot( xaxis, eye ) ,
	  yaxis.x, yaxis.y, yaxis.z, -glm::dot( yaxis, eye ) ,
	  zaxis.x, zaxis.y, zaxis.z, -glm::dot( zaxis, eye ) ,
	  0, 0, 0, 1
	};

	worldMatrix = viewMatrix;

}

void Transform::Update()
{
	pose =  parentMatrix  * worldMatrix;
}

Transform::~Transform()
{

}