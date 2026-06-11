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
		AssetPipeline assetPipeline{};

	public:
		AssetManager assetManager{};
		Engine() = default;
		~Engine() = default;

		AssetPipeline& getAssetImporter();

		InputHandler inputHandler{};
		//called after imports are submitted to asset pipeline, creates asset manager and loads it with imported assets
		void createAssetManager();

		void entityRenderLogic(Entity entity);


	};
}