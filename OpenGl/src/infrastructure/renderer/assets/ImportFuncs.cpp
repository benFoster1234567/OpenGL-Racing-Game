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
	//tinyobj::attrib_t attrib;
	//std::vector<tinyobj::shape_t> shapes;
	//std::vector<tinyobj::material_t> materials;
	//std::string warn, err;

	//if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
	//	throw std::runtime_error(warn + err);
	//}

	//std::vector<Engine::Core::Vertex> vertexData;

	//// Optimize allocation overhead by reserving vector memory upfront
	//size_t totalIndices = 0;
	//for (const auto& shape : shapes) {
	//	totalIndices += shape.mesh.indices.size();
	//}
	//vertexData.reserve(totalIndices);

	//for (const auto& shape : shapes) {
	//	for (const auto& index : shape.mesh.indices) {

	//		// Safeguard: Initialize to defaults in case the .obj lacks normals/texcoords
	//		glm::vec2 texCoords(0.0f, 0.0f);
	//		glm::vec3 normal(0.0f, 1.0f, 0.0f); // Default up-vector normal prevents black artifacting

	//		glm::vec3 position(
	//			attrib.vertices[3 * index.vertex_index + 0],
	//			attrib.vertices[3 * index.vertex_index + 1],
	//			attrib.vertices[3 * index.vertex_index + 2]
	//		);

	//		if (index.normal_index >= 0) {
	//			normal.x = attrib.normals[3 * index.normal_index + 0];
	//			normal.y = attrib.normals[3 * index.normal_index + 1];
	//			normal.z = attrib.normals[3 * index.normal_index + 2];
	//		}

	//		if (index.texcoord_index >= 0) {
	//			texCoords.x = attrib.texcoords[2 * index.texcoord_index + 0];
	//			// FIX: Flip the Y/V texture coordinate vertically for OpenGL compliance
	//			texCoords.y = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1];
	//		}

	//		vertexData.emplace_back(Engine::Core::Vertex{ position, texCoords, normal });
	//	}
	//}

	//Engine::Core::MeshData newMesh{};
	//newMesh.vertices = std::move(vertexData); // Use move semantics to avoid copying large arrays
	//return newMesh;
	return Core::MeshData{};
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
	//std::cout << "From inside the import func: " << sd.shaderSrc << "\n====================================\n";
	return sd;
}
