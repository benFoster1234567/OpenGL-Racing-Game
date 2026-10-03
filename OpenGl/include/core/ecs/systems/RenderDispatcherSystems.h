#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include "../../assets/MeshData.h"
#include "../../assets/ShaderData.h"
#include "../../assets/MaterialData.h"
#include "../../events/EventDispatcher.h"
#include "../../ecs/coordinator/System.h"

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