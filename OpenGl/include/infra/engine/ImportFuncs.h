#pragma once

#include "core/assets/MeshData.h"
#include "core/assets/ShaderData.h"
#include "core/assets/TextureData.h"
#include <core/assets/MaterialData.h>

namespace Engine::Infra::ImportFuncs
{
	Engine::Core::MeshData importMeshDataTOL(const std::string& path, const std::string& name);
	//Engine::Core::MeshData importMeshDataWithIndicesTOL(const std::string& path, const std::string& name);
	Engine::Core::ShaderData importShaderData(const std::string& path, const std::string& name);
	Engine::Core::TextureData importTextureDataDevIL(const std::string& path, const std::string& name);
	Engine::Core::MaterialData importMaterialData(const std::string& path, const std::string& name);
}