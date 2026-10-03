#pragma once
#include "../coordinator/ECS.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>

#include "../../assets/AssetIds.h"
#include "../components/TransformComponent.h"

namespace Engine::Core::ECS
{
	enum class PhysicsEngineCommandType
	{
		CreateRigidbody,
		DeleteRigidbody,
		UpdateTransform,
	};

	struct CreateBoxColliderCommand
	{
		Core::ECS::Entity entity{};
		glm::vec3 bounds{};
	};

	//TODO: Consider replacing the events with a struct called 'RigidBodyState,' which is resposible for carrying data between the physics engine and the core engine. This could simplify some of it I think.

	struct CreateRigidbodyCommand
	{
		Entity entity{};
		Core::MeshId meshId{};
		float mass{};
		bool isKinematic{};
		TransformComponent* transform{};
		glm::vec3 momentOfInertia{};
	};

	struct CreateVehicleSimulationCommand
	{
		Entity entity;
		float mass{};

	};

	struct DeleteRigidbodyCommand
	{
		Entity entity{};
	};

	struct MotionCommand
	{
		Entity entity{};

		glm::vec3 position;
		glm::quat rotation;
	};

	struct PhysicsEngineCommandBuffer
	{
		std::vector<CreateBoxColliderCommand> createBoxes{};
		std::vector<CreateRigidbodyCommand> createBodies{};
		std::vector<DeleteRigidbodyCommand> deleteBodies{};

		std::vector<MotionCommand> manualMotion{};

		void clear()
		{
			createBoxes.clear();
			createBodies.clear();
			deleteBodies.clear();
		}

	};

	enum class PhysicsEventType
	{
		TransformUpdate,
		CollisionStarted,
		CollisionEnded,
		TriggerEntered
	};

	struct PhysicsEvent
	{
		PhysicsEventType eventType = PhysicsEventType::TransformUpdate;
		Entity entityA = 0;
		Entity entityB = 0;

		glm::vec3 position = { 0,0,0 };
		glm::quat rotation = { 1,0,0,0 };
		glm::vec3 contactPoint = { 0,0,0 };
	};
}