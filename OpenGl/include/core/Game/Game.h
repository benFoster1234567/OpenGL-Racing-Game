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
		ECS::RenderDispatcherExternalCamera* externalCameraSystem{};
		ECS::Entity playerEntity{};
		ECS::Entity gridEntity{};

	public:
		using Game::Game;

		void setup() override
		{
			renderDispatcher = coordinator.registerSystem<ECS::RenderDispatcherOrbitalCamera>();
			mouseControl = coordinator.registerSystem<ECS::MouseControlSystem>();
			externalCameraSystem = coordinator.registerSystem<ECS::RenderDispatcherExternalCamera>();

			playerEntity = coordinator.createEntity();
			gridEntity = coordinator.createEntity();
			
			ECS::Signature playerSignature{};
			ECS::Signature gridSignature{};

			coordinator.registerComponent<ECS::CameraComponent>();
			coordinator.registerComponent<ECS::MeshComponent>();
			coordinator.registerComponent<ECS::ShaderComponent>();
			coordinator.registerComponent<ECS::TransformComponent>();
			coordinator.registerComponent<ECS::OrbitalCameraComponent>();
			coordinator.registerComponent<ECS::MouseInputSettings>();


			playerSignature.set(coordinator.getComponentType<ECS::CameraComponent>());
			playerSignature.set(coordinator.getComponentType<ECS::TransformComponent>());
			playerSignature.set(coordinator.getComponentType<ECS::ShaderComponent>());
			playerSignature.set(coordinator.getComponentType<ECS::MeshComponent>());
			playerSignature.set(coordinator.getComponentType<ECS::OrbitalCameraComponent>());
			playerSignature.set(coordinator.getComponentType<ECS::MouseInputSettings>());

			coordinator.setSystemSignature<ECS::RenderDispatcherOrbitalCamera>(playerSignature);
			coordinator.setSystemSignature<ECS::MouseControlSystem>(playerSignature);
			ECS::MeshComponent mesh{};
			assetManager.getMesh(mesh.meshData, "bunny");
			ECS::ShaderComponent shader{};
			assetManager.getShader(shader.shaderData, "shader");

			coordinator.addComponent(playerEntity, mesh);
			coordinator.addComponent(playerEntity, ECS::TransformComponent{});
			coordinator.addComponent(playerEntity, ECS::CameraComponent{});
			coordinator.addComponent(playerEntity, shader);
			coordinator.addComponent(playerEntity, ECS::OrbitalCameraComponent{});
			coordinator.addComponent(playerEntity, ECS::MouseInputSettings{});

			gridSignature.set(coordinator.getComponentType<ECS::TransformComponent>());
			gridSignature.set(coordinator.getComponentType<ECS::MeshComponent>());
			gridSignature.set(coordinator.getComponentType<ECS::ShaderComponent>());

			coordinator.setSystemSignature<ECS::RenderDispatcherExternalCamera>(gridSignature);

			ECS::MeshComponent gridMesh{};
			assetManager.getMesh(gridMesh.meshData, "grid");

			ECS::TransformComponent gridTransform{};
			gridTransform.position = { 0.0f,-1.0f,0.0f };

			coordinator.addComponent(gridEntity, gridMesh);
			coordinator.addComponent(gridEntity, shader);
			coordinator.addComponent(gridEntity, gridTransform);
		}

		void shutdown() override
		{

		}

		//update systems here
		void update(float aspect, MouseInputResource mouseState) override
		{
			mouseControl->update(coordinator, mouseState);
			externalCameraSystem->update(coordinator, aspect, playerEntity);
			renderDispatcher->update(coordinator, aspect);
		}
	};

}


