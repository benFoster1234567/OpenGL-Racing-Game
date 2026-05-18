#pragma once
#include "tiny_obj_loader.h"
#include "Entity.h"
#include "AssetManager.h"
#include <stdexcept>
#include <vector>
#include "Material.h"
#include "MeshData.h"



class EntityBuilder
{
private:
	MeshData parseMesh(const std::string& objFilePath);
public:
	EntityBuilder() = default;

	MeshData* getOrLoadMesh(const std::string& objFilePath, const std::string& objName);

	Entity buildEntity(const std::string& objFilePath, const std::string& objName, Material* material);

};

