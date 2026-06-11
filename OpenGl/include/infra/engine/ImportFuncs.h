#pragma once

#include "core/assets/MeshData.h"
#include "core/assets/ShaderData.h"

namespace Engine::Infra::ImportFuncs
{
	Engine::Core::MeshData importMeshData(const std::string& path, const std::string& name);
	Engine::Core::ShaderData importShaderData(const std::string& path, const std::string& name);
}