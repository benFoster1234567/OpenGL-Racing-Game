#pragma once

#include "core/ecs/ECS.h"
#include "core/assets/assetManager.h"
#include "core/ecs/Coordinator.h"
#include "core/ecs/Systems.h"
#include "core/input/KeyboardInput.h"

namespace Engine::Core::Game
{
	using SceneId = uint8_t;

	struct PlayerEntityCommand
	{
		ECS::Entity entity{};
	};

	class Scene
	{
	protected:
		ECS::Coordinator coordinator{};
		AssetManager& assetManager;
		InputBridge& inputHandler;

	public:
		Scene(AssetManager& _assetManager, InputBridge& _inputHandler) :
			assetManager(_assetManager),
			inputHandler(_inputHandler)
		{
			static SceneId currentId = 0;
			id = currentId;
			currentId++;
		}

		virtual ~Scene() = default;

		SceneId id;

		// import assets, setup entities, components, and systems
		virtual void setup() = 0;

		// clean assetManager, coordinator, etc.
		virtual void shutdown() = 0;

		// called in main loop - updates systems, etc.
		virtual void update(float aspect, MouseInputResource mouseState, float deltaTime) = 0;
	};

	class TestScene : public Scene
	{
	private:
		ECS::Entity playerEntity{};
		ECS::Entity gridEntity{};
		ECS::Entity lightEntity{};

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
			coordinator.registerComponent<ECS::MaterialDataComponent>();
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

			externalCamSig.set(coordinator.getComponentType<ECS::MaterialDataComponent>());
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

			ECS::MeshComponent mesh{ assetManager.getMeshId("bunny") };
			ECS::ShaderComponent shader{ assetManager.getShaderId("shader") };

			ECS::PlayerController playerController{};
			playerController.turnSensitivity = 100;
			playerController.speed = 5;

			ECS::MouseInputSettings mis{};
			mis.sensitivity = { 20, 15 };

			ECS::MaterialDataComponent matComp{};
			assetManager.get(matComp.material, "testMaterial");
			ECS::TransformComponent transform{};

			transform.scale = { 2, 2, 2 };

			if (matComp.material == nullptr)
			{
				throw std::runtime_error("MaterialDataComponent is null for entity " + std::to_string(entity));
			}

			coordinator.addComponent(entity, mesh);
			coordinator.addComponent(entity, transform);
			coordinator.addComponent(entity, ECS::CameraComponent{});
			coordinator.addComponent(entity, shader);
			coordinator.addComponent(entity, ECS::OrbitalCameraComponent{});
			coordinator.addComponent(entity, mis);
			coordinator.addComponent(entity, playerController);
			coordinator.addComponent(entity, matComp);

			return entity;
		}

		ECS::Entity setupGridEntity(ECS::Entity cameraEntity)
		{
			ECS::Entity entity = coordinator.createEntity();

			ECS::ShaderComponent shader{ assetManager.getShaderId("gridShader") };
			ECS::MeshComponent gridMesh{ assetManager.getMeshId("grid") };

			ECS::TransformComponent gridTransform{};
			gridTransform.position = { 0.0f, -1.0f, 0.0f };

			ECS::ExternalCameraComponent extCamComp{};
			extCamComp.entityWithCamera = cameraEntity;

			ECS::MaterialDataComponent matComp{};
			assetManager.get(matComp.material, "testMaterial");

			coordinator.addComponent(entity, gridMesh);
			coordinator.addComponent(entity, shader);
			coordinator.addComponent(entity, gridTransform);
			coordinator.addComponent(entity, extCamComp);
			coordinator.addComponent(entity, matComp);

			return entity;
		}

		ECS::Entity setupCubeEntity(ECS::Entity cameraEntity)
		{
			ECS::Entity entity = coordinator.createEntity();

			ECS::MeshComponent mesh{ assetManager.getMeshId("cube") };
			ECS::ShaderComponent shader{ assetManager.getShaderId("shader") };

			ECS::TransformComponent transform{};
			transform.scale = { 5.5f, 0.2f, 5.5f };
			transform.position = { 0.0f, -1.0f, 0.0f };

			ECS::ExternalCameraComponent extCamComp{};
			extCamComp.entityWithCamera = cameraEntity;

			ECS::MaterialDataComponent matComp{};
			assetManager.get(matComp.material, "cubeMaterial");

			coordinator.addComponent(entity, transform);
			coordinator.addComponent(entity, mesh);
			coordinator.addComponent(entity, shader);
			coordinator.addComponent(entity, extCamComp);
			coordinator.addComponent(entity, matComp);

			return entity;
		}

		ECS::Entity setupLightEntity(ECS::TransformComponent transform, float radius = 20.0f, float intensity = 30.0f)
		{
			ECS::Entity entity = coordinator.createEntity();

			ECS::StaticPointLightComponent lightComp{};
			lightComp.color = { 1, 1, 1 };
			lightComp.radius = radius;
			lightComp.intensity = intensity;

			coordinator.addComponent(entity, transform);
			coordinator.addComponent(entity, lightComp);

			return entity;
		}

		ECS::Entity setupLightEntity()
		{
			ECS::TransformComponent transform{};
			transform.position = { 3, 4, 0 };

			return setupLightEntity(transform, 20.0f);
		}

	public:
		using Scene::Scene;

		void setup() override
		{
			registerSystems();
			registerComponents();
			defineSystemSignatures();

			playerEntity = setupPlayerEntity();
			auto cubeEntity = setupCubeEntity(playerEntity);

			ECS::TransformComponent t{};
			t.position = { 4, 3, -1 };

			auto lightEntity2 = setupLightEntity(t);
			t.position = { -4, 4, 1 };

			//auto lightEntity3 = setupLightEntity(t);
		}

		void setupLights(std::vector<ECS::StaticPointLightRendererData>& lightSetupQueueOut)
		{
			coordinator.getSystem<ECS::StaticLightRenderSetupSystem>()->fill(coordinator, lightSetupQueueOut);
		}

		void shutdown() override
		{

		}

		void update(float aspect, MouseInputResource mouseState, float deltaTime) override
		{
			coordinator.getSystem<ECS::MouseControlSystem>()->update(coordinator, mouseState);
			coordinator.getSystem<ECS::RenderDispatcherExternalCamera>()->update(coordinator, aspect);
			coordinator.getSystem<ECS::RenderDispatcherOrbitalCamera>()->update(coordinator, aspect);
			coordinator.getSystem<ECS::KeyControlSystem>()->update(coordinator, inputHandler, deltaTime);
		}
	};
}