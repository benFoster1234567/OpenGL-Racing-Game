#include "infra/engine/ImportFuncs.h"
#include <utility>
#include <stdexcept>
#include "core/assets/MeshData.h"
#include <memory>
#include "tiny_obj_loader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <core/assets/ShaderData.h>


Engine::Core::MeshData Engine::Infra::ImportFuncs::importMeshData(const std::string& path, const std::string& name)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
		throw std::runtime_error(warn + err);
	}

	Engine::Core::Attribute vertexPoints{}, normals{}, texCoords{};

	std::vector<float> verts{}, norms{}, tex{};

	//pre-optimization -> reserve the space prior to filling to cut down on resize time
	size_t totalIndices = 0;
	for (const auto& shape : shapes) {
		totalIndices += shape.mesh.indices.size();
	}
	verts.reserve(totalIndices);
	norms.reserve(totalIndices);
	tex.reserve(totalIndices);

	bool hasNormals = !attrib.normals.empty();
	bool hasTexcoords = !attrib.texcoords.empty();
	
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {


			verts.push_back(attrib.vertices[3 * index.vertex_index + 0]);
			verts.push_back(attrib.vertices[3 * index.vertex_index + 1]);
			verts.push_back(attrib.vertices[3 * index.vertex_index + 2]);

			if (hasNormals && index.normal_index >= 0)
			{
				norms.push_back(attrib.normals[3 * index.normal_index + 0]);
				norms.push_back(attrib.normals[3 * index.normal_index + 1]);
				norms.push_back(attrib.normals[3 * index.normal_index + 2]);
			}
			else
			{
				norms.push_back(0.0f);
				norms.push_back(1.0f);
				norms.push_back(0.0f);
			}

			if (hasTexcoords && index.texcoord_index >= 0)
			{
				tex.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
				tex.push_back(1.0f - attrib.texcoords[2 * index.texcoord_index + 1]);
			}
			else 
			{
				tex.push_back(0.0f);
				tex.push_back(0.0f);
			}

		}
	}

	//This is going to be slow...
	vertexPoints.data = verts;
	vertexPoints.index = 0;
	vertexPoints.size = 3;

	normals.data = norms;
	normals.index = 1;
	normals.size = 3;

	texCoords.data = tex;
	texCoords.index = 2;
	texCoords.size = 2;

	Engine::Core::MeshData newMesh{};

	newMesh.attributes.push_back(std::move(vertexPoints));//0
	newMesh.attributes.push_back(std::move(normals));//1
	newMesh.attributes.push_back(std::move(texCoords));//2
	return newMesh;
}

Engine::Core::ShaderData Engine::Infra::ImportFuncs::importShaderData(const std::string& path, const std::string& name)
{
	std::ifstream shaderFile(path);

	if (!shaderFile.is_open()) 
	{
		std::cerr << "Failed to open shader file: " << path << std::endl;
		return Core::ShaderData{};
	}

	std::stringstream shaderStream{};
	shaderStream << shaderFile.rdbuf();
	
	Core::ShaderData sd(name, path, shaderStream.str());
	return sd;
}
