

#include <unordered_map>
#include <string>
#include <memory>
#include <optional>

#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Patterns.h"

class AssetManager : Patterns::Singleton<AssetManager>
{
private:
	std::unordered_map<std::string, std::shared_ptr<MeshData>> meshMap;
	std::unordered_map<std::string, std::shared_ptr<Material>> materialMap;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;
	std::unordered_map<std::string, std::shared_ptr<Shader>> shaderMap;
	friend class EntityBuilder;

public:
	// Retrieves a mesh by name. Returns nullptr if the mesh is not found
	std::optional<std::shared_ptr<MeshData>> getMesh(const std::string& name) const;

	std::optional <std::shared_ptr<Material>> getMaterial(const std::string& name) const;
	std::optional <std::shared_ptr<Shader>> getShader(const std::string& name) const;
	std::optional <std::shared_ptr<Texture>> getTexture(const std::string& name) const;


	void addMesh(const std::string& name, std::shared_ptr<MeshData> mesh);
	void addMaterial(const std::string& name, std::shared_ptr<Material> material);
	void addShader(const std::string& name, std::shared_ptr<Shader> shader);
	void addTexture(const std::string& name, std::shared_ptr<Texture> texture);

};

