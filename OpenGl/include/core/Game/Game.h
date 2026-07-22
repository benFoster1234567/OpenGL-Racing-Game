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
		InputBridge& inputHandler;

	public:

		Game(AssetManager& _assetManager, InputBridge& _inputHandler) : 
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
		virtual void update(float aspect, MouseInputResource mouseState) = 0;

	};


	class MainGame : public Game
	{
	private:
		ECS::RenderDispatcherOrbitalCamera* renderDispatcher{};
		ECS::MouseControlSystem* mouseControl{};
	public:
		using Game::Game;

		void setup() override
		{
			renderDispatcher = coordinator.registerSystem<ECS::RenderDispatcherOrbitalCamera>();
			mouseControl = coordinator.registerSystem<ECS::MouseControlSystem>();

			ECS::Entity entity = coordinator.createEntity();

			coordinator.registerComponent<ECS::CameraComponent>();
			coordinator.registerComponent<ECS::MeshComponent>();
			coordinator.registerComponent<ECS::ShaderComponent>();
			coordinator.registerComponent<ECS::TransformComponent>();
			coordinator.registerComponent<ECS::OrbitalCameraComponent>();
			coordinator.registerComponent<ECS::MouseInputSettings>();

			ECS::Signature signature{};

			signature.set(coordinator.getComponentType<ECS::CameraComponent>());
			signature.set(coordinator.getComponentType<ECS::TransformComponent>());
			signature.set(coordinator.getComponentType<ECS::ShaderComponent>());
			signature.set(coordinator.getComponentType<ECS::MeshComponent>());
			signature.set(coordinator.getComponentType<ECS::OrbitalCameraComponent>());
			signature.set(coordinator.getComponentType<ECS::MouseInputSettings>());

			coordinator.setSystemSignature<ECS::RenderDispatcherOrbitalCamera>(signature);
			coordinator.setSystemSignature<ECS::MouseControlSystem>(signature);
			ECS::MeshComponent mesh{};
			assetManager.getMesh(mesh.meshData, "bunny");
			ECS::ShaderComponent shader{};
			assetManager.getShader(shader.shaderData, "shader");

			coordinator.addComponent(entity, mesh);
			coordinator.addComponent(entity, ECS::TransformComponent{});
			coordinator.addComponent(entity, ECS::CameraComponent{});
			coordinator.addComponent(entity, shader);
			coordinator.addComponent(entity, ECS::OrbitalCameraComponent{});
			coordinator.addComponent(entity, ECS::MouseInputSettings{});
		}

		void shutdown() override
		{

		}

		//update systems here
		void update(float aspect, MouseInputResource mouseState) override
		{
			mouseControl->update(coordinator, mouseState);
			renderDispatcher->update(coordinator, aspect);
		}
	};

}


