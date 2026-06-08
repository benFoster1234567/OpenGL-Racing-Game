#pragma once
#include <GL/glew.h>
#include <string>

#include "core/assets/ShaderData.h"


class GpuShader
{
public:
	GpuShader(Engine::Core::ShaderData* _data);

	std::string getName() const { return name; }

	void use() const { glUseProgram(Id);  };

	void loadShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFileName);

	GLuint getId() const { return Id; }

private:
	GLuint Id;
	std::string name;
	Engine::Core::ShaderData* data;

};