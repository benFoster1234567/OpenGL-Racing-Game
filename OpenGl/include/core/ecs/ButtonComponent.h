#pragma once
#include <string>

#include "core/assets/AssetIds.h"
#include "Component.h"

namespace Engine::Core::ECS
{
	struct ButtonComponent : public ComponentBase
	{
		//size_t id{};
		
		TextureId idle{};
		TextureId mouseHover{};
		TextureId mouseClick{};

		size_t pxWidth{};
		size_t pxHeight{};

		size_t positionX{};
		size_t positionY{};

		std::string text{""};

		void setIdle() { state = 0; }
		void setHover() { state = 1; }
		void setClicked() { state = 2; }

		bool isIdle() const { return state == 0; }
		bool isHovered() const { return state == 1; }
		bool isClicked() const { return state == 2; }

	private:
		int state{ 0 };
	};
}
