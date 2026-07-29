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
		virtual void update(float aspect, MouseInputResource mouseState, float deltaTime) = 0;

	};


	class MainGame : public Game
	{
	private:

		ECS::Entity playerEntity{};
		ECS::Entity gridEntity{};
		ECS::Entity lightEntity{ };
		void registerSystems()
		{
			coordinator.registerSystem<ECS::RenderDispatcherOrbitalCamera>();
			coordinator.registerSystem<ECS::RenderDispatcherExternalCamera>();
			coordinator.registerSystem<ECS::KeyControlSystem>();
			coordinator.registerSystem<ECS::MouseControlSystem>();
			coordinator.registerSystem<ECS::StaticLightRenderSetupSystem>();
		}

		void registerComponents()
		{
			coordinator.registerComponent<ECS::CameraComponent>();
			coordinator.registerComponent<ECS::MeshComponent>();
			coordinator.registerComponent<ECS::ShaderComponent>();
			coordinator.registerComponent<ECS::TransformComponent>();
			coordinator.registerComponent<ECS::OrbitalCameraComponent>();
			coordinator.registerComponent<ECS::MouseInputSettings>();
			coordinator.registerComponent<ECS::PlayerController>();
			coordinator.registerComponent<ECS::StaticPointLightComponent>();
			coordinator.registerComponent<ECS::ExternalCameraComponent>();
		}

		void defineSystemSignatures()
		{
			ECS::Signature playerSignature{};

			playerSignature.set(coordinator.getComponentType<ECS::CameraComponent>());
			playerSignature.set(coordinator.getComponentType<ECS::TransformComponent>());
			playerSignature.set(coordinator.getComponentType<ECS::ShaderComponent>());
			playerSignature.set(coordinator.getComponentType<ECS::MeshComponent>());
			playerSignature.set(coordinator.getComponentType<ECS::OrbitalCameraComponent>());
			playerSignature.set(coordinator.getComponentType<ECS::MouseInputSettings>());
			playerSignature.set(coordinator.getComponentType<ECS::PlayerController>());

			coordinator.setSystemSignature<ECS::RenderDispatcherOrbitalCamera>(playerSignature);
			coordinator.setSystemSignature<ECS::MouseControlSystem>(playerSignature);
			coordinator.setSystemSignature<ECS::KeyControlSystem>(playerSignature);

			ECS::Signature externalCamSig{};

			externalCamSig.set(coordinator.getComponentType<ECS::TransformComponent>());
			externalCamSig.set(coordinator.getComponentType<ECS::MeshComponent>());
			externalCamSig.set(coordinator.getComponentType<ECS::ShaderComponent>());
			externalCamSig.set(coordinator.getComponentType<ECS::ExternalCameraComponent>());
			coordinator.setSystemSignature<ECS::RenderDispatcherExternalCamera>(externalCamSig);

			ECS::Signature lightSignature{};
			lightSignature.set(coordinator.getComponentType<ECS::TransformComponent>());
			lightSignature.set(coordinator.getComponentType<ECS::StaticPointLightComponent>());

			coordinator.setSystemSignature<ECS::StaticLightRenderSetupSystem>(lightSignature);

		}

		ECS::Entity setupPlayerEntity()
		{
			ECS::Entity entity = coordinator.createEntity();
		
			ECS::MeshComponent mesh{};
			assetManager.getMesh(mesh.meshData, "bunny");
			ECS::ShaderComponent shader{};
			assetManager.getShader(shader.shaderData, "shader");
			ECS::PlayerController playerController{};

			coordinator.addComponent(entity, mesh);
			coordinator.addComponent(entity, ECS::TransformComponent{});
			coordinator.addComponent(entity, ECS::CameraComponent{});
			coordinator.addComponent(entity, shader);
			coordinator.addComponent(entity, ECS::OrbitalCameraComponent{});
			coordinator.addComponent(entity, ECS::MouseInputSettings{});
			coordinator.addComponent(entity, playerController);

			return entity;
		}

		ECS::Entity setupGridEntity(ECS::Entity cameraEntity)
		{
			ECS::Entity entity = coordinator.createEntity();

			ECS::ShaderComponent shader{};
			assetManager.getShader(shader.shaderData, "shader");

			ECS::MeshComponent gridMesh{};
			assetManager.getMesh(gridMesh.meshData, "grid");

			ECS::TransformComponent gridTransform{};
			gridTransform.position = { 0.0f,-1.0f,0.0f };

			ECS::ExternalCameraComponent extCamComp{};
			extCamComp.entityWithCamera = cameraEntity;

			coordinator.addComponent(entity, gridMesh);
			coordinator.addComponent(entity, shader);
			coordinator.addComponent(entity, gridTransform);
			coordinator.addComponent(entity, extCamComp);

			return entity;
		}

		ECS::Entity setupLightEntity(ECS::TransformComponent transform)
		{
			ECS::Entity entity = coordinator.createEntity();

			ECS::StaticPointLightComponent lightComp{};
			lightComp.color = { 1,1,1 };
			lightComp.radius = 10.0f;

			coordinator.addComponent(entity, transform);
			coordinator.addComponent(entity, lightComp);

			return entity;
		}

		ECS::Entity setupLightEntity()
		{
			ECS::TransformComponent transform{};
			transform.position = { 1,2,0 };

			return setupLightEntity(transform);
		}


	public:
		using Game::Game;

		void setup() override
		{
			registerSystems();
			registerComponents();
			defineSystemSignatures();

			playerEntity = setupPlayerEntity();
			gridEntity = setupGridEntity(playerEntity);
			lightEntity = setupLightEntity();

			ECS::TransformComponent t{};
			t.position = { 10,1,-1 };

			auto lightEntity2 = setupLightEntity(t);
		}

		void setupLights(std::vector<ECS::StaticPointLightRendererData>& lightSetupQueueOut)
		{
			coordinator.getSystem<ECS::StaticLightRenderSetupSystem>()->fill(coordinator, lightSetupQueueOut);
		}

		void shutdown() override
		{

		}

		//update systems here
		void update(float aspect, MouseInputResource mouseState, float deltaTime) override
		{
			coordinator.getSystem<ECS::MouseControlSystem>()->update(coordinator, mouseState);
			coordinator.getSystem<ECS::RenderDispatcherExternalCamera>()->update(coordinator, aspect);
			coordinator.getSystem<ECS::RenderDispatcherOrbitalCamera>()->update(coordinator, aspect);
			coordinator.getSystem<ECS::KeyControlSystem>()->update(coordinator, inputHandler, deltaTime);
		}
	};

}


