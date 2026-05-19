#pragma once
#include "tiny_obj_loader.h"
#include "Entity.h"
#include <stdexcept>
#include <vector>
#include "core/assets/AssetManager.h"
#include "core/assets/MaterialData.h"
#include "core/assets/MeshData.h"

namespace Engine::Core
{

	class EntityBuilder
	{
	private:
		MeshData parseMesh(const std::string& objFilePath);
	public:
		EntityBuilder() = default;

		MeshData* getOrLoadMesh(const std::string& objFilePath, const std::string& objName);

		Entity buildEntity(const std::string& objFilePath, const std::string& objName, MaterialData* material);

	};

}
