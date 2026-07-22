#include "infra/renderer/assets/GpuShader.h"
#include <iostream>
#include <string>
#include <sstream>
#include <core/assets/ShaderData.h>

GpuShader::GpuShader(Engine::Core::ShaderData* _data)
	: data(_data), Id{ 0 }
{
}

static std::string buildShaderString(int version, const std::string& defineAs, std::string rawShaderBody)
{
	std::stringstream ss("");
	ss << version;
	return "#version " + ss.str() + "\n" + "#define " + defineAs + "\n" + rawShaderBody;
}

void GpuShader::compileShaders()
{	
	if (!data)
	{
		std::cerr << "CRITICAL ERROR: Cannot compile shader because 'data' pointer is NULL!\n";
		exit(1);
	}

	GLint success;
	char infoLog[512];

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	std::string vertSrc = buildShaderString(430, "VERTEX_SHADER", data->shaderSrc);
	//std::cout << vertSrc << "\n";
	const char* vertSrcStr = vertSrc.c_str();
	glShaderSource(vertexShader, 1, &vertSrcStr, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	
	if (!success) 
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR: Vertex Shader Compilation Failed\n" << infoLog << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragSrc = buildShaderString(430, "FRAGMENT_SHADER", data->shaderSrc);
	const char* fragSrcStr = fragSrc.c_str();
	glShaderSource(fragmentShader, 1, &fragSrcStr, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	
	if (!success) 
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR: Fragment Shader Compilation Failed\n" << infoLog << std::endl;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR: Shader Program Linking Failed\n" << infoLog << std::endl;
	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	this->Id = shaderProgram;
}
