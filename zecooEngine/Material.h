#pragma once
#include <string> 

#include "Shader.h"
#include "Light.h"
#include "Camera.h"

#include<list>  

class Material
{
public:
	Shader* shader;
	Material(Shader* shader);
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
