#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include "../coordinator/System.h"

namespace Engine::Core::ECS
{
	class Coordinator;

	struct StaticPointLightRendererData
	{
		glm::vec3 m_position{ 0.0f };
		glm::vec3 m_color{ 0.0f };
		float radius{ 0.0f };
		float intensity{ 1.0f };
	};

	class StaticLightRenderSetupSystem : public System
	{
	public:
		void fill(Coordinator& coordinator, std::vector<StaticPointLightRendererData>& queue);
	};

	class ShadowPointSystem : public System
	{
	public:
		std::vector<StaticPointLightRendererData> getShadowCastingPointlights(Coordinator& coordinator);
	};
}