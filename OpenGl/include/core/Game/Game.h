#pragma once
#include "core/ecs/ECS.h"
#include "core/assets/assetManager.h"
#include "core/ecs/Coordinator.h"
#include "core/ecs/Systems.h"
#include "core/input/KeyboardInput.h"

namespace Engine::Core::Game
{
	using GameId = uint8_t;

	struct PlayerEntityCommand
	{
		ECS::Entity entity{};
	};

	class Game
	{
	protected:
		ECS::Coordinator coordinator{};
		AssetManager& assetManager;
		KeyboardBridge& inputHandler;

	public:

		Game(AssetManager& _assetManager, KeyboardBridge& _inputHandler) : 
			assetManager(_assetManager), 
			inputHandler(_inputHandler)
		{ 
			static GameId currentId = 0;
			id = currentId;
			currentId++;
		}

		virtual ~Game() = default;

		GameId id;

		//import assets, setup entities, components, and systems
		virtual void setup() = 0;

		//clean assetManager, coordinator, etc.
		virtual void shutdown() = 0;

		//called in main loop - updates systems, etc.
		virtual void update(float aspect, glm::vec2 mouse) = 0;

	};


	class MainGame : public Game
	{
	private:
		ECS::RenderDispatcher* renderDispatcher{};
	public:
		using Game::Game;

		void setup() override
		{
			renderDispatcher = coordinator.registerSystem<ECS::RenderDispatcher>();
			
			ECS::Entity entity = coordinator.createEntity();

			coordinator.registerComponent<ECS::CameraComponent>();
			coordinator.registerComponent<ECS::MeshComponent>();
			coordinator.registerComponent<ECS::ShaderComponent>();
			coordinator.registerComponent<ECS::TransformComponent>();

			ECS::Signature signature{};

			signature.set(coordinator.getComponentType<ECS::CameraComponent>());
			signature.set(coordinator.getComponentType<ECS::TransformComponent>());
			signature.set(coordinator.getComponentType<ECS::ShaderComponent>());
			signature.set(coordinator.getComponentType<ECS::MeshComponent>());

			coordinator.setSystemSignature<ECS::RenderDispatcher>(signature);

			ECS::MeshComponent mesh{};
			assetManager.getMesh(mesh.meshData, "bunny");

			ECS::ShaderComponent shader{};
			assetManager.getShader(shader.shaderData, "shader");

			coordinator.addComponent(entity, mesh);
			coordinator.addComponent(entity, ECS::TransformComponent{});
			coordinator.addComponent(entity, ECS::CameraComponent{});
			coordinator.addComponent(entity, shader);

		}

		void shutdown() override
		{

		}

		//update systems here
		void update(float aspect, glm::vec2 mouse) override
		{
			renderDispatcher->update(coordinator, aspect);
		}
	};

}


