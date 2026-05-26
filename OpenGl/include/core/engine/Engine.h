#pragma once

#include "core/assets/AssetPipeline.h"
#include "core/ecs/EntityBuilder.h"

namespace Engine::Core
{
	class Engine
	{
	private:
		using EntityId = size_t;
		std::unordered_map<EntityId, Entity> entities{};
		AssetManager assetManager{};
		AssetPipeline assetPipeline{};

	public:
		Engine() = default;
		~Engine() = default;

		AssetPipeline& getAssetImporter();


	};
}