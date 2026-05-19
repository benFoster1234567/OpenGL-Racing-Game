#pragma once
#include <GL/glew.h>
#include <string>


class ShaderProgram
{
public:
	std::string getName() const { return name; }

	void use() const { glUseProgram(Id);  };

	void loadShader(const std::string& Filename, const std::string& fragmentFileName);

	GLuint getId() const { return Id; }

private:
	GLuint Id;
	std::string name;


};