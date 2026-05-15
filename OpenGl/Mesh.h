#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/common.hpp>	
#include <glm/mat4x4.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
};


class Mesh
{
private:
	GLuint VBO = 0, VAO = 0, EBO = 0;
	std::vector<Vertex> vertices;
	//std::vector<unsigned int> indices;
	friend class EntityBuilder;

public:
	Mesh() = default;
	
	void GenBuffers();
	void draw();

};