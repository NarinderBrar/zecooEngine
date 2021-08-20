#pragma once
#include <string> 

#include "Shader.h"
#include "Light.h"
#include "Camera.h"

#include<list>  

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

class Material
{
public:
	//glm::vec3 color = glm::vec3(0.5, 0.5, 0.5);

	Shader* shader;
	Material(Shader* shader, glm::vec4 color);

	int directionalLightId = 0;
	int pointLightId = 0;
	int spotLightId = 0;

	void linkLight(DirectionalLight* light);
	void linkLight(PointLight* light);
	void linkLight(SpotLight* light);
	void linkCamera(Camera* camera);
	void updateLight(DirectionalLight* light);
	void updateLight(PointLight* light);
	void updateLight(SpotLight* light);
	void updateCamera(Camera* camera);
	void apply(glm::mat4 transform);
	void update();

	~Material();

private:
	Camera* camera;
	std::list<DirectionalLight*> directionalLights;
	std::list<PointLight*> pointLights;
	std::list<SpotLight*> spotLighs;
};
