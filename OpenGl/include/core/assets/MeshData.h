#pragma once
#include <iostream>
#include <string>
#include <glm/common.hpp>	
#include <glm/mat4x4.hpp>
#include <vector>
#include <utility>
#include <limits> 

namespace Engine::Core
{
	struct Attribute
	{
		std::vector<float> data;
		unsigned int size;
		unsigned int index;
	};

	struct ExtremeMeasurements
	{
		float minX{	std::numeric_limits<float>::max() }, maxX{ std::numeric_limits<float>::min() };
		float minY{ std::numeric_limits<float>::max() }, maxY{ std::numeric_limits<float>::min() };
		float minZ{ std::numeric_limits<float>::max() }, maxZ{ std::numeric_limits<float>::min() };
	};

	enum class MeshType
	{
		Wireframe, 
		Fill
	};

	class MeshData
	{
	private:

		const size_t VERTEX_ATTRIBUTE	= 0;
		const size_t NORMAL_ATTRIBUTE	= 1;
		const size_t TEXCOORD_ATTRIBUTE	= 2;
		const size_t TANGENT_ATTRIBUTE	= 3;

	public:
		MeshData() = default;
		MeshData(std::vector<Attribute> as) : attributes(std::move(as)) {}
		~MeshData() = default;
		std::string name;
		MeshType meshType{ MeshType::Fill };
		std::vector<Attribute> attributes{};

		void printPoints(); //for debugging purposes...

		void recomputeNormals(int vertexIndex = 0, int normalsIndex = 1);
		void recomputeNormalsSmooth(int vertexIndex = 0, int normalsIndex = 1);

		void computeTangents();
		
		ExtremeMeasurements getMinMaxes()
		{
			ExtremeMeasurements minMax;

			minMax.minX = minMax.minY = minMax.minZ = std::numeric_limits<float>::max();
			minMax.maxX = minMax.maxY = minMax.maxZ = std::numeric_limits<float>::lowest();

			const auto& vertices = attributes[VERTEX_ATTRIBUTE].data;

			for (size_t i = 0; i < vertices.size(); i += 3)
			{
				float x = vertices[i];
				float y = vertices[i + 1];
				float z = vertices[i + 2];

				minMax.minX = std::min(minMax.minX, x);
				minMax.maxX = std::max(minMax.maxX, x);

				minMax.minY = std::min(minMax.minY, y);
				minMax.maxY = std::max(minMax.maxY, y);

				minMax.minZ = std::min(minMax.minZ, z);
				minMax.maxZ = std::max(minMax.maxZ, z);
			}

			return minMax;
		}

		void recomputeNormalsAndTangents()
		{
			recomputeNormals();
			computeTangents();
		}

	};

	class GridData : public MeshData
	{
	public:
		GridData()
		{
			meshType = MeshType::Wireframe;
			std::vector<float> vertices;
			std::vector<float> norms;
			Attribute verts{};
			Attribute normals{};
			int slices = 20;
			float spacing = 1.0f;
			float size = (slices * spacing) / 2.0f;

			for (int i = 0; i <= slices; ++i) {
				float pos = -size + (i * spacing);

				vertices.push_back(pos);   vertices.push_back(0.0f); vertices.push_back(-size);
				vertices.push_back(pos);   vertices.push_back(0.0f); vertices.push_back(size);

				vertices.push_back(-size); vertices.push_back(0.0f); vertices.push_back(pos);
				vertices.push_back(size);  vertices.push_back(0.0f); vertices.push_back(pos);

				for (int i{}; i < 4; i++)
				{
					norms.push_back(0);
					norms.push_back(1);
					norms.push_back(0);
				}
			}

			verts.data = vertices;
			
			verts.index = 0;
			verts.size = 3;

			normals.data = norms;
			normals.size = 3;
			normals.index = 1;

			attributes.push_back(verts);
		}


	};
}