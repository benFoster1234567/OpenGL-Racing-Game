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
		std::vector<Entity> renderPool{};
	public:
		Engine() = default;
		~Engine() = default;

		AssetManager assetManager{};
		AssetPipeline& getAssetImporter();

		InputHandler inputHandler{};
		//called after imports are submitted to asset pipeline, creates asset manager and loads it with imported assets
		void createAssetManager();
		void entityRenderLogic(Entity entity);

		std::vector<Entity> pollEntities() { return renderPool; }

		


	};
}