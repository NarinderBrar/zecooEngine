#include "Material.h"

Material::Material(Shader* _shader, glm::vec4 color)
{
	shader = _shader;

	shader->use();
	shader->setInt("material.diffuse", 0);
	shader->setInt("material.specular", 1);
	shader->setFloat("material.shininess", 32.0f);

	shader->setVec4("color", color);
}

void Material::updateLight(DirectionalLight* light)
{
	shader->use();
	shader->setVec3("dirLight.direction", light->direction);
	shader->setVec3("dirLight.ambient", light->ambient);
	shader->setVec3("dirLight.diffuse", light->diffuse);
	shader->setVec3("dirLight.specular", light->specular);

	directionalLightId++;
}

void Material::linkLight(DirectionalLight* light)
{  
	updateLight(light);
	directionalLights.push_back(light);
}

void Material::updateLight(PointLight* light)
{
	shader->use();
	shader->setVec3("pointLights[" + std::to_string(pointLightId) + "].position", light->position);
	shader->setVec3("pointLights[" + std::to_string(pointLightId) + "].ambient", light->ambient);
	shader->setVec3("pointLights[" + std::to_string(pointLightId) + "].diffuse", light->diffuse);
	shader->setVec3("pointLights[" + std::to_string(pointLightId) + "].specular", light->specular);
	shader->setFloat("pointLights[" + std::to_string(pointLightId) + "].constant", light->constant);
	shader->setFloat("pointLights[" + std::to_string(pointLightId) + "].linear", light->lineartant);
	shader->setFloat("pointLights[" + std::to_string(pointLightId) + "].quadratic", light->quadratic);

	pointLightId++;
}
void Material::linkLight(PointLight* light)
{
	updateLight(light);
	pointLights.push_back(light);
}

void Material::updateLight(SpotLight* light)
{
	shader->use();
	shader->setVec3("spotLight.position", light->position);
	shader->setVec3("spotLight.direction", light->direction);//camera front
	shader->setVec3("spotLight.ambient", light->ambient);
	shader->setVec3("spotLight.diffuse", light->diffuse);
	shader->setVec3("spotLight.specular", light->specular);
	shader->setFloat("spotLight.constant", light->constant);
	shader->setFloat("spotLight.linear", light->lineartant);
	shader->setFloat("spotLight.quadratic", light->quadratic);
	shader->setFloat("spotLight.cutOff", light->cutOff);
	shader->setFloat("spotLight.outerCutOff", light->outerCutOff);

	spotLightId++;
}
void Material::linkLight(SpotLight* light)
{
	updateLight(light);
	spotLighs.push_back(light);
}

void Material::updateCamera(Camera* _camera)
{
	shader->use();
	shader->setVec3("viewPos", _camera->GetPosition());
	shader->setMat4("view", _camera->GetViewMatrix());
	shader->setMat4("projection", _camera->GetPerspectiveProjectionMatrix());
	camera = _camera;
}
void Material::linkCamera(Camera* _camera)
{
	updateCamera(_camera);
}

void Material::update()
{
	if (directionalLights.size() > 0)
	{
		directionalLightId = 0;
		for (std::list <DirectionalLight*> ::iterator it = directionalLights.begin(); it != directionalLights.end(); ++it)
			updateLight(*it);
	}
	if (pointLights.size() > 0 )
	{
		pointLightId = 0;
		for (std::list <PointLight*> ::iterator pl = pointLights.begin(); pl != pointLights.end(); ++pl)
			updateLight(*pl);
	}
	if (spotLighs.size() > 0)
	{
		spotLightId = 0;
		for (std::list <SpotLight*> ::iterator sl = spotLighs.begin(); sl != spotLighs.end(); ++sl)
			updateLight(*sl);
	}

	if (camera == NULL)
		return;

	updateCamera(camera);
}

Material::~Material()
{

}

void Material::apply(glm::mat4 transform)
{
	shader->use();
	shader->setMat4("model", transform);
}