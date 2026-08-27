#include "core/assets/MeshData.h"


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

void Engine::Core::MeshData::computeTangents()
{
	Attribute tangentAttribute{};

	size_t vertexCount = attributes[VERTEX_ATTRIBUTE].data.size() / 3;
	
	std::vector<glm::vec3> tan1{};
	tan1.assign(vertexCount, glm::vec3(0));
	std::vector<glm::vec3> tan2{};
	tan2.assign(vertexCount, glm::vec3(0));

	std::vector<float> vertices = attributes[VERTEX_ATTRIBUTE].data;
	std::vector<float> normals = attributes[NORMAL_ATTRIBUTE].data;
	std::vector<float> texCoords = attributes[TEXCOORD_ATTRIBUTE].data;
	

	for (int v{ 0 }, t{ 0 }; v < vertices.size(); v += 9, t += 6)
	{
		glm::vec3 p1{ vertices[v]
					, vertices[v + 1]
					, vertices[v + 2] };

		glm::vec3 p2{ vertices[v + 3]
					, vertices[v + 4]
					, vertices[v + 5] };

		glm::vec3 p3{ vertices[v + 6]
					, vertices[v + 7]
					, vertices[v + 8] };

		glm::vec2 tc1{ texCoords[t + 0], texCoords[t + 1] };
		glm::vec2 tc2{ texCoords[t + 2], texCoords[t + 3] };
		glm::vec2 tc3{ texCoords[t + 4], texCoords[t + 5] };

		glm::vec3 q1 = p2 - p1;
		glm::vec3 q2 = p3 - p1;

		float s1 = tc2.x - tc1.x;
		float s2 = tc3.x - tc1.x;

		float t1 = tc2.y - tc1.y;
		float t2 = tc3.y - tc1.y;

		float denom = (s1 * t2 - s2 * t1);
		float r = (fabs(denom) > 1e-6f) ? 1.0f / denom : 0.0f;

		glm::vec3 sdir((t2 * q1.x - t1 * q2.x) * r, (t2 * q1.y - t1 * q2.y) * r, (t2 * q1.z - t1 * q2.z) * r);
		glm::vec3 tdir((s1 * q2.x - s2 * q1.x) * r, (s1 * q2.y - s2 * q1.y) * r, (s1 * q2.z - s2 * q1.z) * r);

		size_t i1 = (v / 3) + 0;
		size_t i2 = (v / 3) + 1;
		size_t i3 = (v / 3) + 2;

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
		
	}

	tangentAttribute.data.reserve(vertexCount * 4); 
	tangentAttribute.index = 3;
	tangentAttribute.size = 4;

	for (size_t i = 0; i < vertexCount; ++i)
	{
		glm::vec3 n{ normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2] };
		glm::vec3 t = tan1[i];

		glm::vec3 tangent = glm::normalize(t - n * glm::dot(n, t));

		float w = (glm::dot(glm::cross(n, t), tan2[i]) < 0.0f) ? -1.0f : 1.0f;

		tangentAttribute.data.push_back(tangent.x);
		tangentAttribute.data.push_back(tangent.y);
		tangentAttribute.data.push_back(tangent.z);
		tangentAttribute.data.push_back(w);
	}

	attributes[TANGENT_ATTRIBUTE] = tangentAttribute;
}
