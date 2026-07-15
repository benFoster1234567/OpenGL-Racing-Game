#pragma once

#include "core/assets/AssetPipeline.h"
#include "core/assets/AssetManager.h"
#include "core/input/InputHandler.h"
#include "core/input/Keys.h"
#include "core/ecs/ECS.h"

namespace Engine::Core
{
	constexpr int MAX_ENTITIES{ 100 };



	struct EntityRenderCommand
	{
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 modelTransform;
		ShaderData* shader;
		MeshData* mesh;
	};

	struct GameObjects
	{

		ECS::Entity player{};
		ECS::Entity camera{};
		ECS::Entity grid{};

		float aspect{ 1.0f };
		float deltaTime{ 1.0f };
	};

	class EngineSystem
	{
	private:
		
		float deltaTime{1};

		EntityRenderCommand createRenderCommand(ECS::Entity entity, ECS::Entity camera);
		GameObjects gameObjects{};

	public:
		ECS::EntityComponentSystemManager ecsManager{};
		AssetManager assetManager{};
		InputHandler inputHandler{};

		AssetPipeline assetPipeline{};
		
		void updateDeltaTime(float t)  { gameObjects.deltaTime = t; }
		void updateAspect(float a) { gameObjects.aspect = a; }

		void onKey(KeyCode k, bool pressed);
		
		void updateInputState();

		void createAssetManager();

		void fillEntityRenderList(std::vector<EntityRenderCommand>& entityListOut);


		void setupEcs(glm::mat4 view, glm::mat4 projection);

		EventDispatcher<std::vector<ShaderData*>> shaderDispatcher{};
		EventDispatcher<std::vector<MeshData*>> meshDispatcher{};

		void updateSystems() { ecsManager.updateSystems(); }

		void dispatchAssets();

	};
}