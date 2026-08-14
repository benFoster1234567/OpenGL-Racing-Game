#pragma once
#include <vector>
#include <string>
#include <cstddef>
#include <glm/vec2.hpp>
namespace Engine::Core
{
	struct TextureData
	{
		unsigned int width{0};
		unsigned int height{0};
		unsigned int channels{0};
		std::vector<uint8_t> pixels{};
		std::string name{""};

		glm::vec2 uvTiling{ 1.0f,1.0f };
	};
}

