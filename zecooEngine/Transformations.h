#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <vector>
using namespace std;

class Transformations
{
public:
	vector<glm::mat4> list;
	glm::mat4 matrix;
	
	Transformations();

	glm::mat4 Translate( glm::vec3 offsetVec );
	glm::mat4 Scale( glm::vec3 scaleVec );
	glm::mat4 Rotate( float angle, glm::vec3 axis );
	glm::mat4 getPose( int level );

	int Add();
};
