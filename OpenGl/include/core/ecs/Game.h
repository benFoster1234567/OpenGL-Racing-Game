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
#include "components/VehicleComponent.h"

#include "systems/PhysicsSystem.h"
#include "systems/ControlSystems.h"
#include "systems/LightSystems.h"
#include "systems/VehicleSystem.h"
#include "systems/RenderDispatcherSystems.h"

#include "core/input/KeyboardInput.h"

namespace Engine::Core::Game
{
	using SceneId = uint8_t;

	struct PlayerEntityCommand
	{
		ECS::Entity m_entity{};
	};

	class Scene
	{
	public:
		ECS::Coordinator m_coordinator{};
		AssetManager& m_assetManager;
		InputBridge& m_inputHandler;

	public:
		Scene(AssetManager& _assetManager, InputBridge& _inputHandler) :
			m_assetManager(_assetManager),
			m_inputHandler(_inputHandler)
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
	public:
		ECS::Entity m_playerEntity{};
	private:
		ECS::Entity m_gridEntity{};
		ECS::Entity m_lightEntity{};

		void registerSystems()
		{
			m_coordinator.registerSystem<ECS::RenderDispatcherOrbitalCamera>();
			m_coordinator.registerSystem<ECS::RenderDispatcherExternalCamera>();
			m_coordinator.registerSystem<ECS::KeyControlSystem>();
			m_coordinator.registerSystem<ECS::MouseControlSystem>();
			m_coordinator.registerSystem<ECS::StaticLightRenderSetupSystem>();
			m_coordinator.registerSystem<ECS::ShadowPointSystem>();
			m_coordinator.registerSystem<ECS::PhysicsSystem>();
			m_coordinator.registerSystem<ECS::VehicleSystem>();
		}

		void registerComponents()
		{
			m_coordinator.registerComponent<ECS::CameraComponent>();
			m_coordinator.registerComponent<ECS::MeshComponent>();
			m_coordinator.registerComponent<ECS::ShaderComponent>();
			m_coordinator.registerComponent<ECS::TransformComponent>();
			m_coordinator.registerComponent<ECS::OrbitalCameraComponent>();
			m_coordinator.registerComponent<ECS::MouseInputSettings>();
			m_coordinator.registerComponent<ECS::PlayerController>();
			m_coordinator.registerComponent<ECS::StaticPointLightComponent>();
			m_coordinator.registerComponent<ECS::ExternalCameraComponent>();
			m_coordinator.registerComponent<ECS::MaterialDataComponent>();
			m_coordinator.registerComponent<ECS::ShadowCastComponent>();
			m_coordinator.registerComponent<ECS::PhysicsComponent>();
			m_coordinator.registerComponent<ECS::BoxColliderComponent>();
			m_coordinator.registerComponent<ECS::RigidBodyComponent>();
			m_coordinator.registerComponent<ECS::VehicleComponent>();
		}

		void defineSystemSignatures()
		{
			ECS::Signature playerSignature{};

			playerSignature.set(m_coordinator.getComponentType<ECS::CameraComponent>());
			playerSignature.set(m_coordinator.getComponentType<ECS::TransformComponent>());
			playerSignature.set(m_coordinator.getComponentType<ECS::ShaderComponent>());
			playerSignature.set(m_coordinator.getComponentType<ECS::MeshComponent>());
			playerSignature.set(m_coordinator.getComponentType<ECS::OrbitalCameraComponent>());
			playerSignature.set(m_coordinator.getComponentType<ECS::MouseInputSettings>());
			playerSignature.set(m_coordinator.getComponentType<ECS::PlayerController>());

			m_coordinator.setSystemSignature<ECS::RenderDispatcherOrbitalCamera>(playerSignature);
			m_coordinator.setSystemSignature<ECS::MouseControlSystem>(playerSignature);
			m_coordinator.setSystemSignature<ECS::KeyControlSystem>(playerSignature);

			ECS::Signature externalCamSig{};

			externalCamSig.set(m_coordinator.getComponentType<ECS::MaterialDataComponent>());
			externalCamSig.set(m_coordinator.getComponentType<ECS::TransformComponent>());
			externalCamSig.set(m_coordinator.getComponentType<ECS::MeshComponent>());
			externalCamSig.set(m_coordinator.getComponentType<ECS::ShaderComponent>());
			externalCamSig.set(m_coordinator.getComponentType<ECS::ExternalCameraComponent>());

			m_coordinator.setSystemSignature<ECS::RenderDispatcherExternalCamera>(externalCamSig);

			ECS::Signature lightSignature{};
			lightSignature.set(m_coordinator.getComponentType<ECS::TransformComponent>());
			lightSignature.set(m_coordinator.getComponentType<ECS::StaticPointLightComponent>());

			m_coordinator.setSystemSignature<ECS::StaticLightRenderSetupSystem>(lightSignature);

			ECS::Signature shadowCastingSignature{};

			shadowCastingSignature.set(m_coordinator.getComponentType<ECS::ShadowCastComponent>());
			shadowCastingSignature.set(m_coordinator.getComponentType<ECS::TransformComponent>());
			shadowCastingSignature.set(m_coordinator.getComponentType<ECS::StaticPointLightComponent>());

			m_coordinator.setSystemSignature<ECS::ShadowPointSystem>(shadowCastingSignature);

			ECS::Signature physicsSystemSig{};

			physicsSystemSig.set(m_coordinator.getComponentType<ECS::PhysicsComponent>());
			physicsSystemSig.set(m_coordinator.getComponentType<ECS::BoxColliderComponent>());
			physicsSystemSig.set(m_coordinator.getComponentType<ECS::RigidBodyComponent>());
			physicsSystemSig.set(m_coordinator.getComponentType<ECS::TransformComponent>());
			physicsSystemSig.set(m_coordinator.getComponentType<ECS::MeshComponent>());

			m_coordinator.setSystemSignature<ECS::PhysicsSystem>(physicsSystemSig);

			ECS::Signature vehicleSystemSig{};

			vehicleSystemSig.set(m_coordinator.getComponentType<ECS::VehicleComponent>());
			vehicleSystemSig.set(m_coordinator.getComponentType<ECS::TransformComponent>());

			m_coordinator.setSystemSignature<ECS::VehicleSystem>(vehicleSystemSig);
		}



		ECS::Entity setupGridEntity(ECS::Entity cameraEntity)
		{
			ECS::Entity entity = m_coordinator.createEntity();

			ECS::ShaderComponent shader{ m_assetManager.getShaderId("gridShader") };
			ECS::MeshComponent gridMesh{ m_assetManager.getMeshId("grid") };

			ECS::TransformComponent gridTransform{};
			gridTransform.position = { 0.0f, -1.0f, 0.0f };

			ECS::ExternalCameraComponent extCamComp{};
			extCamComp.entityWithCamera = cameraEntity;

			ECS::MaterialDataComponent matComp{};
			m_assetManager.get(matComp.material, "testMaterial");

			m_coordinator.addComponent(entity, gridMesh);
			m_coordinator.addComponent(entity, shader);
			m_coordinator.addComponent(entity, gridTransform);
			m_coordinator.addComponent(entity, extCamComp);
			m_coordinator.addComponent(entity, matComp);


			return entity;
		}

		ECS::Entity setupCubeEntity(ECS::Entity cameraEntity
			, ECS::TransformComponent transform
			, glm::vec2 uvScale = { 10,10 }
			, bool isKinematic = false
			, std::string material = "cubeMaterial"
			, bool addPhysics = true)
		{

			ECS::Entity entity = m_coordinator.createEntity();

			ECS::MeshComponent mesh{ m_assetManager.getMeshId("cube") };
			mesh.uvScale = uvScale;

			ECS::ShaderComponent shader{ m_assetManager.getShaderId("shader") };

			ECS::ExternalCameraComponent extCamComp{};
			extCamComp.entityWithCamera = cameraEntity;

			ECS::MaterialDataComponent matComp{};
			m_assetManager.get(matComp.material, "cubeMaterial");

			m_coordinator.addComponent(entity, transform);
			m_coordinator.addComponent(entity, mesh);
			m_coordinator.addComponent(entity, shader);
			m_coordinator.addComponent(entity, extCamComp);
			m_coordinator.addComponent(entity, matComp);

			if (addPhysics)
			{
				auto minsMaxes = m_assetManager.getMesh(mesh.meshId)->getMinMaxes();

				float xSize = (minsMaxes.maxX - minsMaxes.minX) / 2 * transform.scale.x;
				float ySize = (minsMaxes.maxY - minsMaxes.minY) / 2 * transform.scale.y;
				float zSize = (minsMaxes.maxZ - minsMaxes.minZ) / 2 * transform.scale.z;

				ECS::BoxColliderComponent collider{};
				collider.halfBounds = { xSize,ySize,zSize };
				ECS::RigidBodyComponent rbComp{};
				rbComp.isKinematic = isKinematic;
				rbComp.isStatic = false;
				rbComp.mass = 1.0f;
				rbComp.momentOfInertia = { 1,1,1 };

				m_coordinator.addComponent(entity, collider);
				m_coordinator.addComponent(entity, rbComp);
				m_coordinator.addComponent(entity, ECS::PhysicsComponent{});
			}

			return entity;

		}

		ECS::Entity setupCubeEntity(ECS::Entity cameraEntity
			, glm::vec3 position
			, glm::vec3 eulerAngles
			, glm::vec3 scale
			, glm::vec2 uvScale = { 10,10 }
			, bool isKinematic = false
			, std::string material = "cubeMaterial")
		{
			glm::vec3 euler{ glm::radians(eulerAngles.x), glm::radians(eulerAngles.y), glm::radians(eulerAngles.z) };
			glm::quat quaternion(euler);

			ECS::TransformComponent transform{};
			transform.position = position;
			transform.rotation = quaternion;
			transform.scale = scale;

			ECS::Entity entity = setupCubeEntity(cameraEntity, transform, uvScale, isKinematic, material);

			return entity;

		}



		ECS::Entity setupGroundEntity(ECS::Entity cameraEntity)
		{

			ECS::TransformComponent transform{};
			transform.scale = { 15.5f, 0.2f, 15.5f };
			transform.position = { 0.0f, -1.0f, 0.0f };

			auto entity = setupCubeEntity(cameraEntity, transform, { 20,20 }, true);

			return entity;
		}

		ECS::Entity setupPlayerEntity()
		{
			ECS::Entity entity = m_coordinator.createEntity();

			ECS::MeshComponent mesh{ m_assetManager.getMeshId("cube") };
			ECS::ShaderComponent shader{ m_assetManager.getShaderId("shader") };

			ECS::PlayerController playerController{};
			playerController.turnSensitivity = 100;
			playerController.speed = 5;

			ECS::MouseInputSettings mis{};
			mis.sensitivity = { 20, 15 };

			ECS::MaterialDataComponent matComp{};
			m_assetManager.get(matComp.material, "testMaterial");
			ECS::TransformComponent transform{};


			transform.scale = { 0.5f, .3f, 1.f };

			if (matComp.material == nullptr)
			{
				throw std::runtime_error("MaterialDataComponent is null for entity " + std::to_string(entity));
			}

			m_coordinator.addComponent(entity, mesh);
			m_coordinator.addComponent(entity, transform);
			m_coordinator.addComponent(entity, ECS::CameraComponent{});

			m_coordinator.addComponent(entity, shader);
			m_coordinator.addComponent(entity, ECS::OrbitalCameraComponent{});
			m_coordinator.addComponent(entity, mis);
			m_coordinator.addComponent(entity, playerController);
			m_coordinator.addComponent(entity, matComp);

			return entity;
		}

		ECS::Entity setupPlayerVehicleEntity()
		{
			ECS::Entity entity = setupPlayerEntity();

			auto& transformComponent = m_coordinator.getComponent<ECS::TransformComponent>(entity);
			auto& meshComponent = m_coordinator.getComponent<ECS::MeshComponent>(entity);

			auto meshMinsMaxes = m_assetManager.getMesh(meshComponent.meshId)->getMinMaxes();

			transformComponent.position = { -3,2,-3 };

			glm::vec3 bounds
			{
				transformComponent.scale.x * (meshMinsMaxes.maxX - meshMinsMaxes.minX) / 2,
				transformComponent.scale.y * (meshMinsMaxes.maxY - meshMinsMaxes.minY) / 2,
				transformComponent.scale.z * (meshMinsMaxes.maxZ - meshMinsMaxes.minZ) / 2
			};

			float connectionHeight = -bounds.y + 0.1;

			ECS::WheelInfo wheel1{};
			wheel1.connectionPoint = { -bounds.x, connectionHeight, bounds.z };
			wheel1.isFrontWheel = true;
			wheel1.radius = 0.2f;
			wheel1.suspensionRestLength = 0.15f;
			wheel1.rollInfluence = 1;

			ECS::WheelInfo wheel2 = wheel1;
			wheel2.connectionPoint = { bounds.x, connectionHeight, bounds.z };
			wheel2.isFrontWheel = true;

			ECS::WheelInfo wheel3 = wheel1;
			wheel3.connectionPoint = { -bounds.x, connectionHeight, -bounds.z };
			wheel3.isFrontWheel = false;

			ECS::WheelInfo wheel4 = wheel1;
			wheel4.connectionPoint = { bounds.x, connectionHeight, -bounds.z };
			wheel4.isFrontWheel = false;

			std::vector<ECS::WheelInfo> wheels{ wheel1, wheel2, wheel3, wheel4 };

			ECS::VehicleComponent vehicleComponent{};
			vehicleComponent.wheels = wheels;
			vehicleComponent.collisionBounds = bounds;
			vehicleComponent.mass = 800.f;

			ECS::TransformComponent tireTransform{};
			tireTransform.scale = { 0.2,0.2,0.2 };
			ECS::Entity wh1 = setupCubeEntity(entity, tireTransform, { 1,1 }, false, "cubeMaterial", false);
			ECS::Entity wh2 = setupCubeEntity(entity, tireTransform, { 1,1 }, false, "cubeMaterial", false);
			ECS::Entity wh3 = setupCubeEntity(entity, tireTransform, { 1,1 }, false, "cubeMaterial", false);
			ECS::Entity wh4 = setupCubeEntity(entity, tireTransform, { 1,1 }, false, "cubeMaterial", false);

			std::vector<ECS::Entity> wheelEntities{ wh1,wh2,wh3,wh4 };

			vehicleComponent.wheelEntities = wheelEntities;

			m_coordinator.addComponent(entity, vehicleComponent);
			return entity;
		}


		ECS::Entity setupLightEntity(ECS::TransformComponent transform, float radius = 20.0f, float intensity = 50.0f)
		{
			ECS::Entity entity = m_coordinator.createEntity();

			ECS::StaticPointLightComponent lightComp{};
			lightComp.color = { 1, 1, 1 };
			lightComp.radius = radius;
			lightComp.intensity = intensity;

			m_coordinator.addComponent(entity, transform);
			m_coordinator.addComponent(entity, lightComp);
			m_coordinator.addComponent(entity, ECS::ShadowCastComponent{});

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

			m_playerEntity = setupPlayerVehicleEntity();
			auto cubeEntity = setupGroundEntity(m_playerEntity);

			ECS::TransformComponent t2{};

			t2.scale = glm::vec3{ 0.5f, 1, 0.5f };
			t2.position = { 0,0,0 };
			t2.rotation = { 0,0,0,1 };

			setupCubeEntity(m_playerEntity, t2, { 1,2 }, true);

			ECS::TransformComponent t{};
			ECS::TransformComponent t3{};
			ECS::TransformComponent t4{};
			ECS::TransformComponent t5{};
			ECS::TransformComponent t6{};
			ECS::TransformComponent t7{};
			ECS::TransformComponent t8{};

			t5.position = { 0,10,0 };
			auto fallingCubeEntity = setupCubeEntity(m_playerEntity, t5, { 1,1 });
			t6.position = { 1,14,0 };
			auto fallingCubeEntity2 = setupCubeEntity(m_playerEntity, t6, { 1,1 });

			t.position = { 9, 7, -1 };
			t3.position = { -18, 7, -1 };
			t4.position = { 1, 7, -2 };
			t7.position = { 4, 7, -10 };
			t8.position = { 6, 7, -8 };

			auto lightEntity1 = setupLightEntity(t3);
			auto lightEntity2 = setupLightEntity(t);

			auto lightEntity3 = setupLightEntity(t4);
			setupLightEntity(t7);
			setupLightEntity(t8);



			m_coordinator.getSystem<ECS::PhysicsSystem>()->fillInitialCommandBuffer(m_coordinator);

		}

		ECS::Entity setupPlayerEntity() const { return m_playerEntity; }

		void pollPhysicsEvents(const std::vector<ECS::PhysicsEvent>& eventQueue)
		{
			m_coordinator.getSystem<ECS::PhysicsSystem>()->pollPhysicsEngine(eventQueue);
		}

		ECS::PhysicsEngineCommandBuffer& getPhysicsEngineCommands()
		{
			return m_coordinator.getSystem<ECS::PhysicsSystem>()->getCommandBuffer();
		}


		void setupLights(std::vector<ECS::StaticPointLightRendererData>& lightSetupQueueOut)
		{
			m_coordinator.getSystem<ECS::StaticLightRenderSetupSystem>()->fill(m_coordinator, lightSetupQueueOut);
		}

		std::vector<ECS::StaticPointLightRendererData> getShadowCastingPointlights()
		{
			return m_coordinator.getSystem<ECS::ShadowPointSystem>()->getShadowCastingPointlights(m_coordinator);
		}

		void shutdown() override
		{

		}

		void updatePhysics(float deltaTime) override
		{
			m_coordinator.getSystem<ECS::PhysicsSystem>()->update(m_coordinator, deltaTime);

		}

		void update(float aspect, MouseInputResource mouseState, float deltaTime) override
		{
			m_coordinator.getSystem<ECS::MouseControlSystem>()->update(m_coordinator, mouseState);
			m_coordinator.getSystem<ECS::RenderDispatcherExternalCamera>()->update(m_coordinator, aspect);
			m_coordinator.getSystem<ECS::RenderDispatcherOrbitalCamera>()->update(m_coordinator, aspect);
			m_coordinator.getSystem<ECS::VehicleSystem>()->update(m_coordinator, m_inputHandler, deltaTime);
		}
	};
}