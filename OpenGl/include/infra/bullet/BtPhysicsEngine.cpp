#include <iostream>

#include "BtPhysicsEngine.h"
#include "../../core/ecs/components/VehicleComponent.h"
#include "../../core/ecs/systems/VehicleSystem.h"
#define PIPELINE_TESTING_MODE

namespace Engine::Infra
{
	void BtPhysicsEngine::destroy()
	{
		if (m_dynamicsWorld)
		{

			for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
			{
				btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
				btRigidBody* body = btRigidBody::upcast(obj);

				if (body && body->getMotionState())
				{
					delete body->getMotionState();
				}

				m_dynamicsWorld->removeCollisionObject(obj);
				delete obj;
			}
		}

		for (auto& collisionShape : m_collisionShapes)
		{
			delete collisionShape;
		}

		delete m_dynamicsWorld;
		delete m_solver;
		delete m_broadPhase;
		delete m_dispatcher;
		delete m_collisionConfiguration;

		m_dynamicsWorld = nullptr;
		m_solver = nullptr;
		m_broadPhase = nullptr;
		m_dispatcher = nullptr;
		m_collisionConfiguration = nullptr;
	}


	void BtPhysicsEngine::loadPhysicsCommands(Core::ECS::PhysicsEngineCommandBuffer commandList)
	{
		m_commandQueue = commandList;
		std::cout << commandList.createBodies.size() << " physics  create bodies commands loaded\n";
	}

	void BtPhysicsEngine::evaluateCommands()
	{
		for (auto& command : m_commandQueue.createBoxes)
		{
			evaluateCreateBoxColliderCommand(command);
		}
		for (auto& command : m_commandQueue.createBodies)
		{
			evaluateCreateRigidbodyCommand(command);
		}
		for (auto& command : m_commandQueue.deleteBodies)
		{
			evaluateDeleteRigidbodyCommand(command);
		}
		for (auto& command : m_commandQueue.manualMotion)
		{
			evaluateMotionCommand(command);
		}
	}

	void BtPhysicsEngine::runSimulation(float deltaTime)
	{
		m_dynamicsWorld->stepSimulation(deltaTime, 10);
	}

	std::vector<Core::ECS::PhysicsEvent> Engine::Infra::BtPhysicsEngine::pollEvents()
	{
		return std::vector<Core::ECS::PhysicsEvent>();
	}

	void BtPhysicsEngine::evaluateMotionCommand(const Core::ECS::MotionCommand& motionCommand)
	{
		Core::ECS::PhysicsEvent newEvent;
	}

	void BtPhysicsEngine::evaluateCreateRigidbodyCommand(const Core::ECS::CreateRigidbodyCommand& command)
	{
		Core::ECS::Entity entity = command.entity;

		Core::ECS::TransformComponent* transform = command.transform;
		btVector3 startOrigin(glmToBt(transform->position));

		btCollisionShape* collisionShape = m_collisionShapes.get(command.entity);
		btVector3 momentOfInertia = glmToBt(command.momentOfInertia);

		float mass = command.mass;

		collisionShape->calculateLocalInertia(mass, momentOfInertia);
		MotionStateForECS* motionState = new MotionStateForECS();
		motionState->m_transform = transform;

		if (command.isKinematic)
		{
			mass = 0.0f;
			momentOfInertia = { 0,0,0 };
		}

		btRigidBody::btRigidBodyConstructionInfo rbInfo{ mass, motionState, collisionShape, momentOfInertia };
		btRigidBody* rigidBody = new btRigidBody(rbInfo);

		if (command.isKinematic)
		{
			rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			rigidBody->setActivationState(DISABLE_DEACTIVATION);
		}

		std::cout << "Rigidbody created for entity " << entity << "/n";

		m_rigidbodies.insert(command.entity, rigidBody);

		m_dynamicsWorld->addRigidBody(rigidBody);
	}

	void BtPhysicsEngine::evaluateDeleteRigidbodyCommand(const Core::ECS::DeleteRigidbodyCommand& command)
	{
		m_dynamicsWorld->removeRigidBody(m_rigidbodies.get(command.entity));
	}

	void BtPhysicsEngine::evaluateCreateBoxColliderCommand(const Core::ECS::CreateBoxColliderCommand& command)
	{
		btVector3 bounds = glmToBt(command.bounds);
		btCollisionShape* box = new btBoxShape(bounds);
		m_collisionShapes.insert(command.entity, box);
	}

	void BtPhysicsEngine::createVehicles(Core::ECS::Coordinator& coordinator)
	{
		const auto& entities = coordinator.getSystem<Core::ECS::VehicleSystem>()->m_entities;

		if (m_vehicleRaycaster == nullptr)
		{
			m_vehicleRaycaster = new btDefaultVehicleRaycaster(m_dynamicsWorld);
		}

		for (const auto& entity : entities)
		{
			btRaycastVehicle::btVehicleTuning tuning;

			auto vehicleComponent = coordinator.getComponent<Core::ECS::VehicleComponent>(entity);
			auto transformComponent = coordinator.getComponent<Core::ECS::TransformComponent>(entity);
			transformComponent.rotation = glm::identity<glm::quat>();

			btTransform startTransform{};

			startTransform.setIdentity();
			startTransform.setOrigin(glmToBt(transformComponent.position));
			startTransform.setRotation(glmToBt(transformComponent.rotation));

			btScalar mass(vehicleComponent.mass);
			btVector3 localInertia{ 0,0,0 };

			btCollisionShape* chassisShape = new btBoxShape(glmToBt(vehicleComponent.collisionBounds));
			chassisShape->calculateLocalInertia(mass, localInertia);
			m_collisionShapes.insert(entity, chassisShape);

			MotionStateForECSNew* motionState = new MotionStateForECSNew{ coordinator, entity };

			btRigidBody::btRigidBodyConstructionInfo cinfo{ mass, motionState, chassisShape, localInertia };
			btRigidBody* chassisBody = new btRigidBody{ cinfo };
			chassisBody->setActivationState(DISABLE_DEACTIVATION);

			m_rigidbodies.insert(entity, chassisBody);
			m_dynamicsWorld->addRigidBody(chassisBody);

			btRaycastVehicle* vehicle = new btRaycastVehicle{ tuning, chassisBody, m_vehicleRaycaster };
			m_vehicles.insert(entity, vehicle);
			vehicle->setCoordinateSystem(0, 1, 2);
			m_dynamicsWorld->addAction(vehicle);

			for (int i = 0; i < vehicleComponent.wheels.size(); i++)
			{
				const auto& wheelInfo = vehicleComponent.wheels[i];
				vehicle->addWheel(glmToBt(wheelInfo.connectionPoint),
					glmToBt(wheelInfo.wheelDir),
					glmToBt(wheelInfo.wheelAxis),
					wheelInfo.suspensionRestLength,
					wheelInfo.radius,
					tuning,
					wheelInfo.isFrontWheel);

				btWheelInfo& wheel = vehicle->getWheelInfo(i);

				wheel.m_suspensionStiffness = wheelInfo.suspensionStiffness;
				wheel.m_wheelsDampingRelaxation = wheelInfo.wheelsDampingRelaxation;
				wheel.m_wheelsDampingCompression = wheelInfo.wheelsDampingCompression;
				wheel.m_frictionSlip = wheelInfo.frictionSlip;
				wheel.m_rollInfluence = wheelInfo.rollInfluence;

			}

		}
	}

	void BtPhysicsEngine::updateVehicles(Core::ECS::Coordinator& coordinator)
	{
		const auto& entities = coordinator.getSystem<Core::ECS::VehicleSystem>()->m_entities;

		for (const auto& entity : entities)
		{
			auto transformComponent = coordinator.getComponent<Core::ECS::TransformComponent>(entity);
			auto& vehicleComponent = coordinator.getComponent<Core::ECS::VehicleComponent>(entity);
			btRaycastVehicle* vehicle = m_vehicles.get(entity);

			for (int i{ 0 }; i < vehicle->getNumWheels(); i++)
			{
				vehicle->applyEngineForce(vehicleComponent.engineForce, i);
				vehicle->setBrake(vehicleComponent.breakingForce, i);
				if (vehicle->getWheelInfo(i).m_bIsFrontWheel)
				{
					vehicle->setSteeringValue(vehicleComponent.steeringValue, i);
				}

				//vehicleComponent.wheels[i].currentSuspensionLength = vehicle->getWheelInfo(i).m_suspensionRestLength1 - vehicle->getWheelInfo(i).m_raycastInfo.m_suspensionLength;
				vehicleComponent.wheels[i].currentSuspensionLength = vehicle->getWheelInfo(i).m_raycastInfo.m_suspensionLength;

			}

		}

	}

}
