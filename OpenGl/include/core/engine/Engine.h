#pragma once

#include "core/assets/AssetPipeline.h"
#include "core/assets/AssetManager.h"
#include "core/input/InputHandler.h"
#include "core/ecs/EntityManager.h"
#include "core/input/Keys.h"

namespace Engine::Core
{
	class Engine
	{
	private:
		EntityManager entityManager{};
		AssetManager assetManager{};
		AssetPipeline assetPipeline{};
		InputHandler inputHandler{};

	public:
		Engine() = default;
		~Engine() = default;

		AssetPipeline& getAssetImporter();
		
		void entityRenderLogic(Entity entity);

	};
}