#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <optional>
#include "MeshData.h"
#include "core/Patterns.h"
#include "core/assets/MaterialData.h"
#include "Shader.h"

namespace Engine::Core {

	class AssetManager : public Patterns::Singleton<AssetManager>
	{
	private:
		std::unordered_map<std::string, std::unique_ptr<MeshData>> meshMap{};
		std::unordered_map<std::string, std::unique_ptr<MaterialData>> materialMap{};
		std::unordered_map<std::string, std::unique_ptr<TextureData>> textureMap{};
		std::unordered_map<std::string, std::unique_ptr<Shader>> shaderMap{};

		friend class EntityBuilder;

	public:
		AssetManager() = default;


		// Retrieves a mesh by name. Returns nullopt if the mesh is not found
		std::optional<MeshData*> getMesh(const std::string& name) const;
		std::optional<MaterialData*> getMaterial(const std::string& name) const;
		std::optional<Shader*> getShader(const std::string& name) const;
		std::optional<TextureData*> getTexture(const std::string& name) const;


		void addMesh(const std::string& name, std::unique_ptr<MeshData> mesh);
		void addMaterial(const std::string& name, std::unique_ptr<MaterialData> material);
		void addShader(const std::string& name, std::unique_ptr<Shader> shader);
		void addTexture(const std::string& name, std::unique_ptr<TextureData> texture);

	};

}