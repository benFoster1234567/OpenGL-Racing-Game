#pragma once
#include "core/assets/MeshData.h"
#include "core/assets/ShaderData.h"
#include "core/events/EventDispatcher.h"
#include "core/ECS/Coordinator.h"
#include <core/input/KeyboardInput.h>
#include <core/input/MouseInputResource.h>
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
		virtual void update(Coordinator& coordinator, float aspect) = 0;
	};

	//this makes the camera follow the entity transform position.
	//requires camera, transform, mesh and shader components
	class RenderDispatcherOrbitalCamera : public RenderDispatcher
	{
	public:
		void update(Coordinator& coordinator, float aspect) override;
	};

	class KeyControlSystem : public System
	{
	public:
		void update(Coordinator& coordinator, const InputBridge& inputHandler)
		{
		}
	};

	class MouseControlSystem : public System
	{
		void update(Coordinator& coordinator, Input::MouseInputResource& mouse)
		{
		}
	};
}