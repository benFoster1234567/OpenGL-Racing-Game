#pragma once

#include "Texture.h"
#include "Shader.h"
#include "AssetManager.h"
#include "Types.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>

class Material
{
private:
	std::string name;

	glm::vec3 ka;
	glm::vec3 ks;
	glm::vec3 kd;
	
	float ni;
	float ns;
	float d;
	int illum;

public:

	std::shared_ptr<Texture> texture;
	std::shared_ptr<Texture> normalMap;
	std::shared_ptr<Shader> shader;
	
	Material(const std::string& name
		, std::shared_ptr<Texture> texture
		, std::shared_ptr<Shader> shader
		, std::shared_ptr<Texture> normalMap = nullptr
		, glm::vec3 ka = glm::vec3(0), glm::vec3 ks = glm::vec3(0), glm::vec3 kd = glm::vec3(0)
		, float ni = 1.0f, float ns = 1.0f, float d = 1.0f, int illum = 1);

	Material(const std::string& name
		, const std::string& texturePath
		, std::shared_ptr<Shader> shader
		, const std::string& normalMapPath = ""
		, glm::vec3 ka = glm::vec3(0), glm::vec3 ks = glm::vec3(0), glm::vec3 kd = glm::vec3(0)
		, float ni = 1.0f, float ns = 1.0f, float d = 1.0f, int illum = 1);


	Material(const std::string& name
		, const std::string& texturePath
		, const std::string& shaderVertexPath
		, const std::string& shaderFragmentPath
		, const std::string& normalMapPath = ""
		, glm::vec3 ka = glm::vec3(0), glm::vec3 ks = glm::vec3(0), glm::vec3 kd = glm::vec3(0)
		, float ni = 1.0f, float ns = 1.0f, float d = 1.0f, int illum = 1);


	// we get a material imported from a mtl file, we need to parse the file and create the material
	TextureStatus importFromStream(std::ifstream& stream);
	TextureStatus importFromFile(const std::string& mtlFilePath);

};

