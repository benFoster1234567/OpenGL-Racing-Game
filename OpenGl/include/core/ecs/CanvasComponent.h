#pragma once

#include "ButtonComponent.h"
#include "Component.h"

namespace Engine::Core::ECS
{
	struct CanvasComponent : public ComponentBase
	{
		size_t width{};
		size_t height{};

		size_t positionX{};
		size_t positionY{};

		float opacity{ 0.0f };

		TextureId backgroundTexture{};

		std::vector<ButtonComponent> buttons{};
	};
}