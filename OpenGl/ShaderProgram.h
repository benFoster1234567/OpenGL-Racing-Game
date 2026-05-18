#pragma once
#include <GL/glew.h>
#include <string>


class ShaderProgram
{
public:
	std::string name;
	void use();
	void loadShader(std::string fileName);
	GLuint getId();

private:
	GLuint Id;

};