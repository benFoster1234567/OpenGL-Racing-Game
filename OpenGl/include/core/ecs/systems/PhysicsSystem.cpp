#include "PhysicsSystem.h"

#include "core/ecs/coordinator/Coordinator.h"
#include "../components/MotionPropertiesComponent.h"
#include "../components/TransformComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/MeshComponent.h"
#include "../components/Colliders.h"

namespace Engine::Core::ECS
{
	void PhysicsSystem::update(Coordinator& coordinator, float deltaTime)
	{
		commandBuffer.clear();

		for (Entity entity : entities)
		{
			auto transform = coordinator.getComponent<TransformComponent>(entity);
			MotionCommand pec
			{
				.entity = entity,
				.position = transform.position,
				.rotation = transform.rotation
			};
			commandBuffer.manualMotion.push_back(pec);
		}
	}

	void PhysicsSystem::fillInitialCommandBuffer(Coordinator& coordinator)
	{
		std::cout << "Physics: Filling initial command buffer: " << entities.size() << " entities\n";
		for (auto& entity : entities)
		{
			auto& rigidBodyComponent = coordinator.getComponent<RigidBodyComponent>(entity);
			auto& meshComponent = coordinator.getComponent<MeshComponent>(entity);
			TransformComponent* transformComponent = &coordinator.getComponent<TransformComponent>(entity);
			auto& boxColliderComponent = coordinator.getComponent<BoxColliderComponent>(entity);

			CreateRigidbodyCommand crc
			{
				.entity = entity,
				.meshId = meshComponent.meshId,
				.mass = rigidBodyComponent.mass,
				.isKinematic = rigidBodyComponent.isKinematic,
				.transform = transformComponent,
				.momentOfInertia = rigidBodyComponent.momentOfInertia
			};

			CreateBoxColliderCommand cbcc
			{
				.entity = entity,
				.bounds = boxColliderComponent.halfBounds,
			};

			commandBuffer.createBoxes.push_back(cbcc);
			commandBuffer.createBodies.push_back(crc);

		}
	}

	void PhysicsSystem::pollPhysicsEngine(const std::vector<PhysicsEvent>& eventList)
	{
		eventQueue = eventList;
	}

}