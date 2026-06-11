#pragma once
#include <string>


namespace Engine::Core
{
	//Pretty much a pointless class, aside from the fact that it helps us keep the shader seperate from the infrastructure.
	class ShaderData
	{
	private:
	public:
		ShaderData() = default;
		ShaderData(const std::string& name, const std::string& filePath, const std::string& shaderStr);

		std::string shaderSrc{};
		std::string filePath{};
		std::string name{};
	};
}