#pragma once
#include "tiny_obj_loader.h"
#include "Mesh.h"
#include "Entity.h"
#include "AssetManager.h"
#include <stdexcept>
#include <vector>


class MeshLoader
{

};


class EntityBuilder
{
private:
	Mesh parseMesh(const std::string& objFilePath);
public:
	EntityBuilder() = default;
	
	std::shared_ptr<Mesh> getOrLoadMesh(const std::string& objFilePath, const std::string& objName);

	Entity buildEntity(const std::string& objFilePath, const std::string& objName, std::shared_ptr<Material> material);

};

