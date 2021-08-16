#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
	public:
		glm::mat4 pose = glm::mat4(1.0f);

		Transform();

		void position(glm::vec3 vec);
	
		void translate(glm::vec3 vec);
		void rotate(float angle, glm::vec3 axis);
		void scale(glm::vec3 vec);
	
		~Transform();
};

