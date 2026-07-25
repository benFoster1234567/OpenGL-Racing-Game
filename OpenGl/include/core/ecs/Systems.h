#pragma once
#include "core/assets/MeshData.h"
#include "core/assets/ShaderData.h"
#include "core/events/EventDispatcher.h"
#include "core/ECS/Coordinator.h"
#include <core/input/KeyboardInput.h>
#include "core/ecs/System.h"


namespace Engine::Core::ECS
{

	struct RenderOutput
	{
		glm::mat4 view{1.0f};
		glm::mat4 projection{1.0f};
		glm::mat4 modelTransform{1.0f};
		ShaderData* shader{};
		MeshData* mesh{};
	};

	class RenderDispatcher : public System
	{
	public:
		inline static EventDispatcher<RenderOutput> sendRenderInfo; // connect renderer to this //
	};

	//this makes the camera follow the entity transform position.
	//requires camera, transform, mesh and shader components
	class RenderDispatcherOrbitalCamera : public RenderDispatcher
	{
	public:
		void update(Coordinator& coordinator, float aspect);
	};

	class RenderDispatcherExternalCamera : public RenderDispatcher
	{
	public:
		void update(Coordinator& coordinator, float aspect, Entity cameraEntity);
	};

	class KeyControlSystem : public System
	{
	public:
		void update(Coordinator& coordinator, const InputBridge& inputHandler)
		{
			for (auto entity : entities)
			{
				const auto& inputKeys = coordinator.getComponent<PlayerController>(entity);
				auto& transform = coordinator.getComponent<TransformComponent>(entity);
				glm::vec3 rotAxis{ 0.0f,1.0f,0.0f };

				if (inputHandler.keyHeld(int(inputKeys.strafeLeft)))
				{

				}
				if (inputHandler.keyHeld(int(inputKeys.strafeRight)))
				{

				}
				if (inputHandler.keyHeld(int(inputKeys.forward)))
				{

				}
				if (inputHandler.keyHeld(int(inputKeys.backward)))
				{

				}
			}
		}
	};

	class MouseControlSystem : public System
	{
	public:
		void update(Coordinator& coordinator, MouseInputResource& mouse);
	};
}