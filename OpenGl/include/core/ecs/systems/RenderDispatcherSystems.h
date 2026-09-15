#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include "core/assets/MeshData.h"
#include "core/assets/ShaderData.h"
#include "core/assets/MaterialData.h"
#include "core/events/EventDispatcher.h"
#include "core/ecs/coordinator/System.h"

namespace Engine::Core::ECS
{
	class Coordinator;

	struct RenderOutput
	{
		glm::mat4 view{ 1.0f };
		glm::mat4 projection{ 1.0f };
		glm::mat4 modelTransform{ 1.0f };
		glm::vec2 uvScale{ 1, 1 };
		ShaderId shader{};
		MeshId mesh{};
		MaterialData* material{};
	};

	class RenderDispatcher : public System
	{
	public: //Get rid of this
		inline static EventDispatcher<RenderOutput> sendRenderInfo;
	};

	class RenderDispatcherOrbitalCamera : public RenderDispatcher
	{
	public:
		void update(Coordinator& coordinator, float aspect);
	};

	class RenderDispatcherExternalCamera : public RenderDispatcher
	{
	public:
		void update(Coordinator& coordinator, float aspect);
	};
}