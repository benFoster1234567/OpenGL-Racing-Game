#pragma once
#include <core/assets/MeshData.h>

namespace Engine::Infra::ImportFuncs
{
	Core::MeshData importMeshData(const std::string& path, const std::string& name);
}