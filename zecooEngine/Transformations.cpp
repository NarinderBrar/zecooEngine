#include "Transformations.h"

Transformations::Transformations()
{
	matrix = glm::mat4( 1.0 );
}

glm::mat4 Transformations::Translate( glm::vec3 offsetVec )
{
	matrix = glm::mat4( 1.0 );
	matrix = glm::translate( matrix, offsetVec );
	return matrix;
}
glm::mat4 Transformations::Scale( glm::vec3 scaleVec )
{
	matrix = glm::mat4( 1.0 );
	matrix = glm::scale( matrix, scaleVec );
	return matrix;
}

glm::mat4 Transformations::Rotate( float angle, glm::vec3 axis )
{
	matrix = glm::mat4( 1.0 );
	matrix = glm::rotate( matrix, glm::degrees(angle), axis );
	return matrix;
}

glm::mat4 Transformations::getPose(int level)
{
	glm::mat4 I = glm::mat4(1.0);

	if( level >= list.size() )
		level = list.size();

	for( int i = level-1; i >= 0; i-- )
	{
		I = I * list[i];
	}

	return I;
}

int Transformations::Add()
{
	list.push_back( matrix );
	return list.size() - 1;
}