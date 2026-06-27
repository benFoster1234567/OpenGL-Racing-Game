#pragma once

#include "core/assets/AssetPipeline.h"
#include "core/assets/AssetManager.h"
#include "core/input/InputHandler.h"
#include "core/ecs/EntityManager.h"
#include "core/input/Keys.h"

namespace Engine::Core
{
	class EngineSystem
	{
	private:
		EntityManager entityManager{};
		std::vector<EntityId> renderQueue{};
		float deltaTime{1};

	public:
		AssetManager assetManager{};
		EngineSystem() = default;
		~EngineSystem() = default;

		AssetPipeline assetPipeline{};
		InputHandler inputHandler{};
		
		void setDeltaTime(float t)  { deltaTime = t; }

		void createAssetManager();

		void fillEntityRenderList(std::vector<Entity*>& entityListOut);

		void updateRenderQueue();
		void createEntities();

		EventDispatcher<std::vector<ShaderData*>> shaderDispatcher{};
		EventDispatcher<std::vector<MeshData*>> meshDispatcher{};

		void dispatchAssets();

	};
}