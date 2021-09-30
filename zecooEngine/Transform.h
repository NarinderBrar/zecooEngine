#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
	public:
		glm::mat4 pose = glm::mat4(1.0f);

		glm::mat4 worldMatrix = glm::mat4(1.0f);

		glm::mat4 localTranslationMatrix = glm::mat4(1.0f);
		glm::mat4 localRotationMatrix = glm::mat4(1.0f);
		glm::mat4 localScaleMatrix = glm::mat4(1.0f);

		Transform();

		void position(glm::vec3 vec);
		void setParent(Transform* parent);

		void translate(glm::vec3 vec);
		void rotate(float angle, glm::vec3 axis);
		void scale(glm::vec3 vec);

		void localTranslate(glm::vec3 vec);

		void localRotate(float angle, glm::vec3 axis);

		void localScale(glm::vec3 vec);

		void Transformation();

		~Transform();
};

