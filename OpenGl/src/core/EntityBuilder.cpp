#include "core/ecs/EntityBuilder.h"
#include <utility>
#include <stdexcept>
#include "core/assets/MeshData.h"
#include <memory>
#include "core/ecs/Entity.h"

using namespace Engine::Core;

MeshData EntityBuilder::parseMesh(const std::string& objFilePath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "model.obj")) {
		throw std::runtime_error(warn + err);
	}

	std::vector<Vertex> vertexData;
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {

			glm::vec2 texCoords(0.0f);
			glm::vec3 normal(0.0f);

			glm::vec3 position(
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			);

			if (index.normal_index >= 0) {
				normal.x = attrib.normals[3 * index.normal_index + 0];
				normal.y = attrib.normals[3 * index.normal_index + 1];
				normal.z = attrib.normals[3 * index.normal_index + 2];
			}

			if (index.texcoord_index >= 0) {
				texCoords.x = attrib.texcoords[2 * index.texcoord_index + 0];
				texCoords.y = attrib.texcoords[2 * index.texcoord_index + 1];
			}

			vertexData.emplace_back(position, texCoords, normal);
		}
	}

	MeshData newMesh{};
	newMesh.vertices = vertexData;

	return newMesh;
}

MeshData* EntityBuilder::getOrLoadMesh(const std::string& objFilePath, const std::string& objName)
{
	auto existingMesh = AssetManager::getInstance().getMesh(objName);
	if (existingMesh.has_value()) {
		return existingMesh.value();
	}
	AssetManager::getInstance().addMesh(objName, std::make_unique<MeshData>(parseMesh(objFilePath)));
	return AssetManager::getInstance().getMesh(objName).value();
}



Entity EntityBuilder::buildEntity(const std::string& objFilePath, const std::string& objName, MaterialData* material)
{
	return 	Entity(getOrLoadMesh(objFilePath, objName), material);
}


