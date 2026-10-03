#pragma once

#include <glm/vec2.hpp>
#include "../../assets/MeshData.h"
#include "../coordinator/ECS.h"
#include "../../assets/AssetIds.h"


namespace Engine::Core::ECS
{
	struct MeshComponent : public ComponentBase
	{
		MeshComponent() = default;
		MeshComponent(Engine::Core::MeshId id) : meshId(id) {}
		Engine::Core::MeshId meshId{};
		glm::vec2 uvScale{ 1, 1 };
	};
}