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

	class GridData : public MeshData
	{
	public:
		GridData()
		{
			std::vector<float> vertices;
			int slices = 20;
			float spacing = 1.0f;
			float size = (slices * spacing) / 2.0f;

			for (int i = 0; i <= slices; ++i) {
				float pos = -size + (i * spacing);

				vertices.push_back(pos);   vertices.push_back(0.0f); vertices.push_back(-size);
				vertices.push_back(pos);   vertices.push_back(0.0f); vertices.push_back(size);

				vertices.push_back(-size); vertices.push_back(0.0f); vertices.push_back(pos);
				vertices.push_back(size);  vertices.push_back(0.0f); vertices.push_back(pos);
			}

			Attribute verts{};
			verts.data = vertices;
			
			verts.index = 0;
			verts.size = 3;

			attributes.push_back(verts);
		}


	};
}