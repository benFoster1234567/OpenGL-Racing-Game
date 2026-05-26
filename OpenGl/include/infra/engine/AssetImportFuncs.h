#pragma once
//path and name of asset to import, type of asset to import
#include "core/assets/AssetPipeline.h"

namespace Engine::Infra
{
	template<typename T>
	Core::AssetVariant importAsset(const std::string& path)
	{
		return T;
	}
	
	

}