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
	public:
		AssetManager assetManager{};
		EngineSystem() = default;
		~EngineSystem() = default;

		AssetPipeline assetPipeline{};
		InputHandler inputHandler{};

		void createAssetManager();

		EventDispatcher<std::vector<ShaderData*>> shaderDispatcher{}; //the meshes and shaders get sent to the application layer through here?
		EventDispatcher<std::vector<MeshData*>> meshDispatcher{}; //

		void dispatchAssets();

	};
}