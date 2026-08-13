#pragma once
#include <vector>
#include <string>
#include <cstddef>

namespace Engine::Core
{
	struct TextureData
	{
		unsigned int width{0};
		unsigned int height{0};
		unsigned int channels{0};
		std::vector<uint8_t> pixels{};
		std::string name{""};
	};
}

