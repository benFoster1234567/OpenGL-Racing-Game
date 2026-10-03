#pragma once
#include <vector>

#include <stdexcept>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

#include "../../core/ecs/systems/PhysicsCommandsAndEvents.h"
#include "../../core/ecs/components/TransformComponent.h"
#include "../../SparseSet.h"
#include "../../core/ecs/coordinator/Coordinator.h"

//TODO: Implement Bullet3 into infrastructure -- detect collisions, vehicle physics


namespace Engine::Infra
{
	//helpers
	static btVector3 glmToBt(const glm::vec3& v)
	{
		return btVector3{ v.x, v.y, v.z };
	}

	static btQuaternion glmToBt(const glm::quat& q)
	{
		return btQuaternion{ q.x, q.y, q.z, q.w };
	}

	static glm::vec3 btToGlm(const btVector3& v)
	{
		return glm::vec3{ v.x(), v.y(), v.z() };
	}

	static glm::quat btToGlm(const btQuaternion& q)
	{
		return glm::quat{ q.w(), q.x(), q.y(), q.z() };
	}
	//

	class MotionStateForECS : public btMotionState
	{
	public:
		Core::ECS::TransformComponent* m_transform{ nullptr };

		void getWorldTransform(btTransform& worldTrans) const override
		{
			if (m_transform == nullptr) return;
			worldTrans.setOrigin(glmToBt(m_transform->position));
			worldTrans.setRotation(glmToBt(m_transform->rotation));
		}

		void setWorldTransform(const btTransform& worldTrans)
		{
			if (m_transform == nullptr) return;
			m_transform->position = btToGlm(worldTrans.getOrigin());
			m_transform->rotation = btToGlm(worldTrans.getRotation());
		}
	};

	class MotionStateForECSNew : public btMotionState
	{
	public:
		Core::ECS::Coordinator& m_coordinator;
		Core::ECS::Entity m_entity{};

		MotionStateForECSNew(Core::ECS::Coordinator& coordinator, Core::ECS::Entity entity) : m_coordinator{ coordinator }, m_entity{ entity } {}

		void getWorldTransform(btTransform& worldTrans) const override
		{
			auto& transform = m_coordinator.getComponent<Core::ECS::TransformComponent>(m_entity);
			worldTrans.setOrigin(glmToBt(transform.position));
			worldTrans.setRotation(glmToBt(transform.rotation));
		}

		void setWorldTransform(const btTransform& worldTrans)
		{
			auto& transform = m_coordinator.getComponent<Core::ECS::TransformComponent>(m_entity);
			transform.position = btToGlm(worldTrans.getOrigin());
			transform.rotation = btToGlm(worldTrans.getRotation());
		}

	};



	class BtPhysicsEngine
	{
	private:

		btDefaultCollisionConfiguration* m_collisionConfiguration{ nullptr };
		btCollisionDispatcher* m_dispatcher{ nullptr };
		btDbvtBroadphase* m_broadPhase{ nullptr };
		btSequentialImpulseConstraintSolver* m_solver{ nullptr };
		btDiscreteDynamicsWorld* m_dynamicsWorld{ nullptr };
		btVehicleRaycaster* m_vehicleRaycaster{ nullptr };

		Core::ECS::PhysicsEngineCommandBuffer m_commandQueue{};
		std::vector<Core::ECS::PhysicsEvent> m_eventCache{};


		void evaluateMotionCommand(const Core::ECS::MotionCommand& motionCommand);
		void evaluateCreateRigidbodyCommand(const Core::ECS::CreateRigidbodyCommand& command);
		void evaluateDeleteRigidbodyCommand(const Core::ECS::DeleteRigidbodyCommand& command);
		void evaluateCreateBoxColliderCommand(const Core::ECS::CreateBoxColliderCommand& command);

		static constexpr size_t MAX_COMPONENTS = 128;
		static constexpr size_t MAX_ENTITIES = 1000;
		SparseSet<btCollisionShape*, MAX_ENTITIES, MAX_ENTITIES> m_collisionShapes{};

		SparseSet<btRigidBody*, MAX_ENTITIES, MAX_ENTITIES> m_rigidbodies{};
		SparseSet<btRaycastVehicle*, MAX_ENTITIES, MAX_ENTITIES> m_vehicles{};


	public:

		BtPhysicsEngine()
		{
			m_broadPhase = new btDbvtBroadphase();
			m_collisionConfiguration = new btDefaultCollisionConfiguration();
			m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
			m_solver = new btSequentialImpulseConstraintSolver();
			m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadPhase, m_solver, m_collisionConfiguration);
			m_dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
		}

		~BtPhysicsEngine()
		{
			destroy();
		}

		void createVehicles(Core::ECS::Coordinator& coordinator);
		void updateVehicles(Core::ECS::Coordinator& coordinator);

		BtPhysicsEngine(const BtPhysicsEngine&) = delete;
		BtPhysicsEngine& operator=(const BtPhysicsEngine&) = delete;

		BtPhysicsEngine(BtPhysicsEngine&& other) noexcept
			: m_collisionConfiguration(std::exchange(other.m_collisionConfiguration, nullptr)),
			m_dispatcher(std::exchange(other.m_dispatcher, nullptr)),
			m_broadPhase(std::exchange(other.m_broadPhase, nullptr)),
			m_solver(std::exchange(other.m_solver, nullptr)),
			m_dynamicsWorld(std::exchange(other.m_dynamicsWorld, nullptr)),
			m_collisionShapes(std::move(other.m_collisionShapes)),
			//	commandQueue(std::exchange(other.commandQueue, nullptr)),
			m_eventCache(std::move(other.m_eventCache))
		{
		}


		void destroy();

		void loadPhysicsCommands(Core::ECS::PhysicsEngineCommandBuffer commandList);

		void evaluateCommands();

		void runSimulation(float deltaTime); // runs the simulation

		std::vector<Core::ECS::PhysicsEvent> pollEvents();

	};
}