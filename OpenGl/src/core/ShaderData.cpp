#include "core/assets/ShaderData.h"
#include <string>

Engine::Core::ShaderData::ShaderData(const std::string& name, const std::string& filePath, const std::string& shaderStr)
	: name{name}, filePath{filePath}, shaderSrc{shaderStr}
{
}
