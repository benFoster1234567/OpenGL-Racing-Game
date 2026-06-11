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

	void compileShaders();

	GLuint getId() const { return Id; }
	GLuint Id;

private:
	std::string name;
	Engine::Core::ShaderData* data;

};