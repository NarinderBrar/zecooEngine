#pragma once
#include "Shader.h";
#include "Material.h";
#include "Texture.h";
#include "Transform.h";

class Model
{

private:
	//Shader* shader;
	Material* material;
	Texture* texture;

public:
	std::string name = "";
	Transform* transform ;

	Model(Material* material, Texture* texture);

	void render();

	~Model();
};

