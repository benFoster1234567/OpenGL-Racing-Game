#pragma once
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/common.hpp>	
#include <glm/mat4x4.hpp>
#include <vector>

namespace Engine::Core
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texCoords;
		glm::vec3 normal;
	};


	class MeshData
	{
	private:
	public:
		MeshData() = default;
		~MeshData() = default;
		std::string name;
		std::vector<Vertex> vertices;
	};
}