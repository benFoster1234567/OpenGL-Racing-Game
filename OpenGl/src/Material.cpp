#include "Material.h"

Material::Material(const std::string& name
	, std::shared_ptr<Texture> texture
	, std::shared_ptr<Shader> shader
	, std::shared_ptr<Texture> normalMap
	, glm::vec3 ka, glm::vec3 ks, glm::vec3 kd
	, float ni, float ns, float d, int illum)
	: name(name), texture(texture), shader(shader), normalMap(normalMap)
		, ka(ka), ks(ks), kd(kd), ni(ni), ns(ns), d(d), illum(illum)
{
}

Material::Material(const std::string& name
	, const std::string& texturePath
	, std::shared_ptr<Shader> shader
	, const std::string& normalMapPath
	, glm::vec3 ka, glm::vec3 ks, glm::vec3 kd
	, float ni, float ns, float d, int illum)
	: name(name), shader(shader), ka(ka), ks(ks)
		, kd(kd), ni(ni), ns(ns), d(d), illum(illum)
{
	texture = std::make_shared<Texture>();
	texture->loadTexture(texturePath.c_str());
	if (!normalMapPath.empty())
	{
		normalMap = std::make_shared<Texture>();
		normalMap->loadTexture(normalMapPath.c_str());
	}
}

Material::Material(const std::string& name
	, const std::string& texturePath
	, const std::string& shaderVertexPath
	, const std::string& shaderFragmentPath
	, const std::string& normalMapPath
	, glm::vec3 ka, glm::vec3 ks, glm::vec3 kd
	, float ni, float ns, float d, int illum)
	: name(name), ka(ka), ks(ks), kd(kd), ni(ni), ns(ns), d(d), illum(illum)
{
	texture = std::make_shared<Texture>();
	texture->loadTexture(texturePath.c_str());
	shader = std::make_shared<Shader>(shaderVertexPath.c_str(), shaderFragmentPath.c_str());
	if (!normalMapPath.empty())
	{
		normalMap = std::make_shared<Texture>();
		normalMap->loadTexture(normalMapPath.c_str());
	}
}

TextureStatus Material::importFromStream(std::ifstream& stream)
{
	if(!stream) {
		std::cerr << "Failed to open material stream." << std::endl;
		return TextureStatus::NotFound;
	}

	if (!stream.is_open()) {
		std::cerr << "Material stream is not open." << std::endl;
		return TextureStatus::NotFound;
	}
	std::string line;

	std::optional<std::string> map_Kd = nullptr;
	std::optional<std::string> map_Normal = nullptr;

	while (std::getline(stream, line)) 
	{

		std::stringstream ss(line);
		std::string token;
		std::vector<std::string> row;

		while (std::getline(ss, token, ' ')) {
			row.push_back(token);
		}

		auto rowToVec3 = [&]() -> glm::vec3
		{
			if (row.capacity() >= 4)
				return glm::vec3(row[1], row[2], row[3]);
			else return glm::vec3(0);
		};

		if (row[0] == "Ka") ka = rowToVec3();
		else if (row[0] == "Ks") ks = rowToVec3();
		else if (row[0] == "Kd") kd = rowToVec3();
		else if (row[0] == "Ns") ns = std::stof(row[1]);
		else if (row[0] == "Ni") ni = std::stof(row[1]);
		else if (row[0] == "d") d = std::stof(row[1]);
		else if (row[0] == "illum") illum = std::stoi(row[1]);
		else if (row[0] == "map_Kd") map_Kd.emplace(row[1]);
		else if (row[0] == "map_Normal") map_Normal.emplace(row[1]); 

	}

	return TextureStatus::Loaded;
}
