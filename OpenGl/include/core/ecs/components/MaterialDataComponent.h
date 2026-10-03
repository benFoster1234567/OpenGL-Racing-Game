#pragma once

#include "../../assets/MaterialData.h"
#include "../coordinator/ECS.h"


namespace Engine::Core::ECS
{
	struct MaterialDataComponent : public ComponentBase
	{
		MaterialDataComponent() = default;
		MaterialDataComponent(MaterialData* _material) : material{ _material } {}
		~MaterialDataComponent() = default;
		MaterialData* material{ nullptr };
	};
}