#pragma once
#include <iostream>
#include <string>
#include <glm/common.hpp>	
#include <glm/mat4x4.hpp>
#include <vector>
#include <utility>

namespace Engine::Core
{
	struct Attribute
	{
		std::vector<float> data;
		unsigned int size;
		unsigned int index;
	};

	class MeshData
	{
	private:
	public:
		MeshData() = default;
		MeshData(std::vector<Attribute> as) : attributes(std::move(as)) {}
		~MeshData() = default;
		std::string name;
		
		std::vector<Attribute> attributes{};
		void printPoints(); //for debugging purposes...


	};
}