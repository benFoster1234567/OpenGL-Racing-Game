#pragma once
#include <vector>

#include <stdexcept>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

#include "../../core/ecs/systems/PhysicsCommandsAndEvents.h"
#include "../../core/ecs/components/TransformComponent.h"
#include "../../SparseSet.h"


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
		return btQuaternion{ q.w, q.x, q.y, q.z };
	}

	static glm::vec3 btToGlm(const btVector3& v)
	{
		return glm::vec3{ v.x(), v.y(), v.z()};
	}

	static glm::quat btToGlm(const btQuaternion& q)
	{
		return glm::quat{ q.w(), q.x(), q.y(), q.z() };
	}
	//

	class MotionStateForECS : public btMotionState
	{
	public:
		Core::ECS::TransformComponent* transform{nullptr};

		void getWorldTransform(btTransform& worldTrans) const override
		{
			if (transform == nullptr) return;
			worldTrans.setOrigin(glmToBt(transform->position));
			worldTrans.setRotation(glmToBt(transform->rotation));
		}

		void setWorldTransform(const btTransform& worldTrans)
		{
			if (transform == nullptr) return;
			transform->position = btToGlm(worldTrans.getOrigin());
			transform->rotation = btToGlm(worldTrans.getRotation());
		}
	};

	class BtPhysicsEngine
	{
	private:

		btDefaultCollisionConfiguration* collisionConfiguration{nullptr};
		btCollisionDispatcher* dispatcher{nullptr};
		btDbvtBroadphase* broadPhase{nullptr};
		btSequentialImpulseConstraintSolver* solver{nullptr};
		btDiscreteDynamicsWorld* dynamicsWorld{nullptr};

		Core::ECS::PhysicsEngineCommandBuffer commandQueue{};
		std::vector<Core::ECS::PhysicsEvent> eventCache{};

		
		void evaluateMotionCommand(const Core::ECS::MotionCommand& motionCommand);
		void evaluateCreateRigidbodyCommand(const Core::ECS::CreateRigidbodyCommand& command);
		void evaluateDeleteRigidbodyCommand(const Core::ECS::DeleteRigidbodyCommand& command);
		void evaluateCreateBoxColliderCommand(const Core::ECS::CreateBoxColliderCommand& command);
		
		static constexpr size_t MAX_COMPONENTS = 128;
		static constexpr size_t MAX_ENTITIES = 1000;
		SparseSet<btCollisionShape*, MAX_ENTITIES, MAX_ENTITIES> collisionShapes{};

		SparseSet<btRigidBody*, MAX_ENTITIES, MAX_ENTITIES> rigidbodies{};

	public:

		BtPhysicsEngine()
		{
			broadPhase = new btDbvtBroadphase();
			collisionConfiguration = new btDefaultCollisionConfiguration();
			dispatcher = new btCollisionDispatcher(collisionConfiguration);
			solver = new btSequentialImpulseConstraintSolver();
			dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfiguration); 
			dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
		}

		~BtPhysicsEngine()
		{
			destroy();
		}


		BtPhysicsEngine(const BtPhysicsEngine&) = delete;
		BtPhysicsEngine& operator=(const BtPhysicsEngine&) = delete;

		BtPhysicsEngine(BtPhysicsEngine&& other) noexcept
			: collisionConfiguration(std::exchange(other.collisionConfiguration, nullptr)),
			dispatcher(std::exchange(other.dispatcher, nullptr)),
			broadPhase(std::exchange(other.broadPhase, nullptr)),
			solver(std::exchange(other.solver, nullptr)),
			dynamicsWorld(std::exchange(other.dynamicsWorld, nullptr)),
			collisionShapes(std::move(other.collisionShapes)),
		//	commandQueue(std::exchange(other.commandQueue, nullptr)),
			eventCache(std::move(other.eventCache))
		{
		}
		

		void destroy();

		void loadPhysicsCommands(Core::ECS::PhysicsEngineCommandBuffer commandList);

		void evaluateCommands();

		void runSimulation(float deltaTime); // runs the simulation

		std::vector<Core::ECS::PhysicsEvent> pollEvents();

	};
}