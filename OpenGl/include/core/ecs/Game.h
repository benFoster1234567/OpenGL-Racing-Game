#pragma once

#include "coordinator/ECS.h"
#include "core/assets/assetManager.h"
#include "coordinator/Coordinator.h"

#include "components/CameraComponents.h"
#include "components/MaterialDataComponent.h"
#include "components/MeshComponent.h"
#include "components/MotionPropertiesComponent.h"
#include "components/MouseInputSettingsComponent.h"
#include "components/PlayerControllerComponent.h"
#include "components/PointlightComponents.h"
#include "components/ShaderComponent.h"
#include "components/TransformComponent.h"
#include "components/PhysicsComponent.h"
#include "components/RigidBodyComponent.h"
#include "components/Colliders.h"

#include "systems/PhysicsSystem.h"
#include "systems/ControlSystems.h"
#include "systems/LightSystems.h"
#include "systems/RenderDispatcherSystems.h"

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

		virtual void updatePhysics(float deltaTime) = 0;
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
			coordinator.registerSystem<ECS::ShadowPointSystem>();
			coordinator.registerSystem<ECS::PhysicsSystem>();
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
			coordinator.registerComponent<ECS::ShadowCastComponent>();
			coordinator.registerComponent<ECS::PhysicsComponent>();
			coordinator.registerComponent<ECS::BoxColliderComponent>();
			coordinator.registerComponent<ECS::RigidBodyComponent>();
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

			ECS::Signature shadowCastingSignature{};

			shadowCastingSignature.set(coordinator.getComponentType<ECS::ShadowCastComponent>());
			shadowCastingSignature.set(coordinator.getComponentType<ECS::TransformComponent>());
			shadowCastingSignature.set(coordinator.getComponentType<ECS::StaticPointLightComponent>());

			coordinator.setSystemSignature<ECS::ShadowPointSystem>(shadowCastingSignature);

			ECS::Signature physicsSystemSig{};

			physicsSystemSig.set(coordinator.getComponentType<ECS::PhysicsComponent>());
			physicsSystemSig.set(coordinator.getComponentType<ECS::BoxColliderComponent>());
			physicsSystemSig.set(coordinator.getComponentType<ECS::RigidBodyComponent>());
			physicsSystemSig.set(coordinator.getComponentType<ECS::TransformComponent>());
			physicsSystemSig.set(coordinator.getComponentType<ECS::MeshComponent>());

			coordinator.setSystemSignature<ECS::PhysicsSystem>(physicsSystemSig);
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


		ECS::Entity setupRbCubeEntity(ECS::Entity cameraEntity, ECS::TransformComponent transform, glm::vec2 uvScale)
		{
			ECS::Entity entity = coordinator.createEntity();

			ECS::MeshComponent mesh{ assetManager.getMeshId("cube") };
			mesh.uvScale = uvScale;
			ECS::ShaderComponent shader{ assetManager.getShaderId("shader") };


			ECS::ExternalCameraComponent extCamComp{};
			extCamComp.entityWithCamera = cameraEntity;

			ECS::MaterialDataComponent matComp{};
			assetManager.get(matComp.material, "cubeMaterial");

			coordinator.addComponent(entity, transform);
			coordinator.addComponent(entity, mesh);
			coordinator.addComponent(entity, shader);
			coordinator.addComponent(entity, extCamComp);
			coordinator.addComponent(entity, matComp);


			auto minsMaxes = assetManager.getMesh(mesh.meshId)->getMinMaxes();

			float xSize = (minsMaxes.maxX - minsMaxes.minX) / 2 * transform.scale.x;
			float ySize = (minsMaxes.maxY - minsMaxes.minY) / 2 * transform.scale.y;
			float zSize = (minsMaxes.maxZ - minsMaxes.minZ) / 2 * transform.scale.z;

			ECS::BoxColliderComponent collider{};
			collider.halfBounds = { xSize,ySize,zSize };
			ECS::RigidBodyComponent rbComp{};
			rbComp.isKinematic = false;
			rbComp.isStatic = false;
			rbComp.mass = 1.0f;
			rbComp.momentOfInertia = { 1,1,1 };

			coordinator.addComponent(entity, collider);
			coordinator.addComponent(entity, rbComp);
			coordinator.addComponent(entity, ECS::PhysicsComponent{});
			

			return entity;
		}


		ECS::Entity setupGroundEntity(ECS::Entity cameraEntity)
		{
			ECS::Entity entity = coordinator.createEntity();

			ECS::MeshComponent mesh{ assetManager.getMeshId("cube") };
			mesh.uvScale = { 20,20 };

			ECS::ShaderComponent shader{ assetManager.getShaderId("shader") };

			ECS::TransformComponent transform{};
			transform.scale = { 15.5f, 0.2f, 15.5f };
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

			auto minsMaxes = assetManager.getMesh(mesh.meshId)->getMinMaxes();

			float xSize = (minsMaxes.maxX - minsMaxes.minX) / 2 * transform.scale.x;
			float ySize = (minsMaxes.maxY - minsMaxes.minY) / 2 * transform.scale.y;
			float zSize = (minsMaxes.maxZ - minsMaxes.minZ) / 2 * transform.scale.z;

			ECS::BoxColliderComponent collider{};
			collider.halfBounds = { xSize,ySize,zSize };
			ECS::RigidBodyComponent rbComp{};
			rbComp.isKinematic = true;
			rbComp.isStatic = false;
			rbComp.mass = 1.0f;
			rbComp.momentOfInertia = { 1,1,1 };

			coordinator.addComponent(entity, collider);
			coordinator.addComponent(entity, rbComp);
			coordinator.addComponent(entity, ECS::PhysicsComponent{});

			return entity;
		}

		ECS::Entity setupCubeEntity(ECS::Entity cameraEntity, ECS::TransformComponent transform, glm::vec2 uvScale)
		{
			ECS::Entity entity = coordinator.createEntity();

			ECS::MeshComponent mesh{ assetManager.getMeshId("cube") };
			mesh.uvScale = uvScale;
			ECS::ShaderComponent shader{ assetManager.getShaderId("shader") };


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

		ECS::Entity setupLightEntity(ECS::TransformComponent transform, float radius = 25.0f, float intensity = 180.0f)
		{
			ECS::Entity entity = coordinator.createEntity();

			ECS::StaticPointLightComponent lightComp{};
			lightComp.color = { 1, 1, 1 };
			lightComp.radius = radius;
			lightComp.intensity = intensity;

			coordinator.addComponent(entity, transform);
			coordinator.addComponent(entity, lightComp);
			coordinator.addComponent(entity, ECS::ShadowCastComponent{});

			return entity;
		}

		ECS::Entity setupLightEntity()
		{
			ECS::TransformComponent transform{};
			transform.position = { 15, 10, 0 };

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
			auto cubeEntity = setupGroundEntity(playerEntity);

			ECS::TransformComponent t2{};

			t2.scale = glm::vec3{ 0.5f, 1, 0.5f };
			t2.position = { 0,0,0 };
			t2.rotation = { 0,0,0,1 };

			setupRbCubeEntity(playerEntity, t2, { 1,2 });

			ECS::TransformComponent t{};
			ECS::TransformComponent t3{};
			ECS::TransformComponent t4{};
			ECS::TransformComponent t5{};
			ECS::TransformComponent t6{};

			t5.position = { 0,10,0 };
			auto fallingCubeEntity = setupRbCubeEntity(playerEntity, t5, {1,1});
			t6.position = { 1,14,0 };
			auto fallingCubeEntity2 = setupRbCubeEntity(playerEntity, t6, {1,1});

			t.position = { 18, 7, -1 };
			t3.position = { -18, 7, -1 };
			t4.position = { 1, 7, -2 };
			auto lightEntity1 = setupLightEntity(t3);
			auto lightEntity2 = setupLightEntity(t);

			auto lightEntity3 = setupLightEntity(t4);

			coordinator.getSystem<ECS::PhysicsSystem>()->fillInitialCommandBuffer(coordinator);

		}

		void pollPhysicsEvents(const std::vector<ECS::PhysicsEvent>& eventQueue)
		{
			coordinator.getSystem<ECS::PhysicsSystem>()->pollPhysicsEngine(eventQueue);
		}

		ECS::PhysicsEngineCommandBuffer& getPhysicsEngineCommands()
		{
			return coordinator.getSystem<ECS::PhysicsSystem>()->getCommandBuffer();
		}


		void setupLights(std::vector<ECS::StaticPointLightRendererData>& lightSetupQueueOut)
		{
			coordinator.getSystem<ECS::StaticLightRenderSetupSystem>()->fill(coordinator, lightSetupQueueOut);
		}

		std::vector<ECS::StaticPointLightRendererData> getShadowCastingPointlights()
		{
			return coordinator.getSystem<ECS::ShadowPointSystem>()->getShadowCastingPointlights(coordinator);
		}

		void shutdown() override
		{

		}

		void updatePhysics(float deltaTime) override
		{
			coordinator.getSystem<ECS::PhysicsSystem>()->update(coordinator, deltaTime);

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