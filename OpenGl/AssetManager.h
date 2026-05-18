

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
	std::unordered_map<std::string, std::unique_ptr<MeshData>> meshMap{};
	std::unordered_map<std::string, std::unique_ptr<Material>> materialMap{};
	std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap{};
	std::unordered_map<std::string, std::unique_ptr<Shader>> shaderMap{};
	
	friend class EntityBuilder;

public:
	AssetManager() = default;


	// Retrieves a mesh by name. Returns nullopt if the mesh is not found
	std::optional<MeshData*> getMesh(const std::string& name) const;
	std::optional<Material*> getMaterial(const std::string& name) const;
	std::optional<Shader*> getShader(const std::string& name) const;
	std::optional<Texture*> getTexture(const std::string& name) const;


	void addMesh(const std::string& name, std::unique_ptr<MeshData> mesh);
	void addMaterial(const std::string& name, std::unique_ptr<Material> material);
	void addShader(const std::string& name, std::unique_ptr<Shader> shader);
	void addTexture(const std::string& name, std::unique_ptr<Texture> texture);

};

