#pragma once
#include <GL/glew.h>
#include <string>

#include "core/assets/ShaderData.h"


class GpuShader
{
public:
	GpuShader(Engine::Core::ShaderData* _data);

	std::string getName() const { return m_name; }

	void use() const { glUseProgram(m_id); };

	void compileShaders();

	GLuint getId() const { return m_id; }
	GLuint m_id;

private:
	std::string m_name;
	Engine::Core::ShaderData* m_data;

};