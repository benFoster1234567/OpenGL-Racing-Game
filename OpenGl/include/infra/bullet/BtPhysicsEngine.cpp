#include <iostream>

#include "BtPhysicsEngine.h"
#define PIPELINE_TESTING_MODE

namespace Engine::Infra
{


	void BtPhysicsEngine::destroy()
	{
        if (dynamicsWorld)
        {

            for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
            {
                btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
                btRigidBody* body = btRigidBody::upcast(obj);

                if (body && body->getMotionState())
                {
                    delete body->getMotionState();
                }

                dynamicsWorld->removeCollisionObject(obj);
                delete obj;
            }
        }

        for (auto& collisionShape : collisionShapes)
        {
            delete collisionShape;
        }
        
        delete dynamicsWorld;
        delete solver;
        delete broadPhase;
        delete dispatcher;
        delete collisionConfiguration;

		dynamicsWorld = nullptr;
        solver = nullptr;
        broadPhase = nullptr;
        dispatcher = nullptr;
        collisionConfiguration = nullptr;
	}


	void BtPhysicsEngine::loadPhysicsCommands(Core::ECS::PhysicsEngineCommandBuffer commandList)
	{
		commandQueue = commandList;
        std::cout << commandList.createBodies.size() << " physics  create bodies commands loaded\n";
	}

    void BtPhysicsEngine::evaluateCommands()
    {
        for (auto& command : commandQueue.createBoxes)
        {
            evaluateCreateBoxColliderCommand(command);
        }
        for (auto& command : commandQueue.createBodies)
        {
            evaluateCreateRigidbodyCommand(command);
        }
        for (auto& command : commandQueue.deleteBodies)
        {
            evaluateDeleteRigidbodyCommand(command);
        }
        for (auto& command : commandQueue.manualMotion)
        {
            evaluateMotionCommand(command);
        }
    }

	void BtPhysicsEngine::runSimulation(float deltaTime)
	{
        dynamicsWorld->stepSimulation(deltaTime, 10);
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

        btCollisionShape* collisionShape = collisionShapes.get(command.entity);
        btVector3 momentOfInertia = glmToBt(command.momentOfInertia);

        float mass = command.mass;

        collisionShape->calculateLocalInertia(mass, momentOfInertia);
        MotionStateForECS* motionState = new MotionStateForECS();
        motionState->transform = transform;
        
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

        rigidbodies.insert(command.entity, rigidBody);

        dynamicsWorld->addRigidBody(rigidBody);
    }

    void BtPhysicsEngine::evaluateDeleteRigidbodyCommand(const Core::ECS::DeleteRigidbodyCommand& command)
    {
        dynamicsWorld->removeRigidBody(rigidbodies.get(command.entity));
    }

    void BtPhysicsEngine::evaluateCreateBoxColliderCommand(const Core::ECS::CreateBoxColliderCommand& command)
    {
        btVector3 bounds = glmToBt(command.bounds);
        btCollisionShape* box = new btBoxShape(bounds);
        collisionShapes.insert(command.entity, box);
    }

}
