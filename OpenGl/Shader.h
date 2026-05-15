#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/common.hpp>
#include <glm/mat4x4.hpp>
class Shader
{
	GLuint *ID;
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	void use();
	void setMat4(const char* name, const float* value);
};

