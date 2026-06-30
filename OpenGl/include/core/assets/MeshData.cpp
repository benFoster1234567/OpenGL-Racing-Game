#include "MeshData.h"


void Engine::Core::MeshData::printPoints()
{
	for (const auto& a : attributes)
	{
		std::cout << "attribute index [" << a.index << "]\n";
		for (int i{}; i < a.data.size(); i++)
		{
			std::cout << a.data[i];
			if (i == a.data.size() - 1)
				std::cout << "\n\n";
			else if (i >= 1000) 
			{
				std::cout << "...\n\n";
				break;
			}
			else
				std::cout << ", ";
		}
	}
}

void Engine::Core::MeshData::recomputeNormals(int vertexIndex, int normalsIndex)
{
	//for every 3 vertices, is one vector.
	//every 9 vectors is a triangle
	//take the cross product of 2 vectors from the same triangle, normalize it, and that is our normal for those three points. 
	//So then we repeat the values for the normal three times to match with the indices of the vertex.

	if (normalsIndex >= attributes.size() || vertexIndex >= attributes.size()) return;

	auto verts = attributes[vertexIndex].data;

	std::vector<float> norms{};

	for (int i{}; i < verts.size();)
	{

		glm::vec3 vert1{ verts[i], verts[i + 1], verts[i + 2] }; i += 3;
		glm::vec3 vert2{ verts[i], verts[i + 1], verts[i + 2] }; i += 3;
		glm::vec3 vert3{ verts[i], verts[i + 1], verts[i + 2] }; i += 3;

		glm::vec3 norm = glm::normalize(glm::cross(vert2 - vert1, vert3 - vert1));

		for (int i{}; i < 3; i++)
		{
			norms.push_back(norm.x);
			norms.push_back(norm.y);
			norms.push_back(norm.z);
		}

	}

	attributes[normalsIndex].data = norms;



}
