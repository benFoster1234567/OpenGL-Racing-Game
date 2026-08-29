#pragma once
#include "core/assets/MeshData.h"
#include "core/assets/ShaderData.h"
#include "core/events/EventDispatcher.h"
#include "core/ECS/Coordinator.h"
#include "core/ECS/StaticPointlightComponent.h"
#include <core/input/KeyboardInput.h>
#include "core/ecs/System.h"


namespace Engine::Core::ECS
{
	struct RenderOutput
	{
		glm::mat4 view{1.0f};
		glm::mat4 projection{1.0f};
		glm::mat4 modelTransform{1.0f};
		glm::vec2 uvScale{1,1};
		ShaderId shader{};
		MeshId mesh{};
		MaterialData* material{};
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
		//void update(Coordinator& coordinator, float aspect, Entity cameraEntity);
		void update(Coordinator& coordinator, float aspect);
	};

	class KeyControlSystem : public System
	{
	public:
		void update(Coordinator& coordinator, const InputBridge& inputHandler, float deltaTime);
	};

	class MouseControlSystem : public System
	{
	public:
		void update(Coordinator& coordinator, MouseInputResource& mouse);
	};

	struct StaticPointLightRendererData
	{
		glm::vec3 position{ 0.0f };
		glm::vec3 color{ 0.0f };
		float radius{ 0.0f };
		float intensity{ 1.0f };
	};

	class StaticLightRenderSetupSystem : public System
	{
	public:
		void fill(Coordinator& coordinator, std::vector<StaticPointLightRendererData>& queue);
	};

	class PhysicsSystem : public System
	{
	public:
		void update(Coordinator& coordinator, float deltaTime);
	};
}