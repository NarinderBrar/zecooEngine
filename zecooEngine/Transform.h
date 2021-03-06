#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
	private:

	public:
		glm::mat4 worldMatrix = glm::mat4( 1.0f );
		glm::mat4 parentMatrix = glm::mat4( 1.0f );

		glm::mat4 pose = glm::mat4(1.0f);
		glm::vec3 cubPos = glm::vec3( 0, 0, 0 );
		//glm::mat4 localTranslationMatrix = glm::mat4( 1.0f );
		//glm::mat4 localRotationMatrix = glm::mat4( 1.0f );
		//glm::mat4 localScaleMatrix = glm::mat4( 1.0f );

		glm::mat4 worldScaleMatrix = glm::mat4( 1.0f );

		glm::vec3 localScale = glm::vec3(1.0f,1.0f,1.0f);
		glm::vec3 eulerAngle = glm::vec3(0.0f,0.0f,0.0f);

		Transform* parent;

		Transform();

		void position(glm::vec3 vec);

		glm::vec3 getPosition();

		glm::vec3 getlocalXDir();
		glm::vec3 getlocalYDir();
		glm::vec3 getlocalZDir();

		void setParent(Transform* _parent);

		void translate(glm::vec3 vec);
		void rotate(float angle, glm::vec3 axis);
		void scale(glm::vec3 vec);

		void worldTranslate(glm::vec3 vec);

		void worldRotate(float angle, glm::vec3 axis);

		void worldScale(glm::vec3 vec);
		void resetParentScale( glm::vec3 vec );
		void lookAt( glm::vec3 eye, glm::vec3 at, glm::vec3 up );
		
		void Update();

		void MoveTowards( float dt, Transform* target, float speed, float offset );
		
		~Transform();
};

