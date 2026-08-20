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
#include <IL/il.h>


Engine::Core::MeshData Engine::Infra::ImportFuncs::importMeshDataTOL(const std::string& path, const std::string& name)
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

	newMesh.name = name;
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

// later I should refactor this to support more options.
Engine::Core::TextureData Engine::Infra::ImportFuncs::importTextureDataDevIL(const std::string& path, const std::string& name)
{
	static bool isDevILInitialized = false;

	if (!isDevILInitialized) {
		ilInit();
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		isDevILInitialized = true;
	}

	ILuint imageID;

	ilGenImages(1, &imageID);

	ilBindImage(imageID);

	if (!ilLoadImage(reinterpret_cast<const ILchar*>(path.c_str()))) 
	{
		std::cerr << "Failed to open material file: " << path << std::endl;
		return Engine::Core::TextureData{};
	}

	if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) 
	{
		std::cerr << "Failed to convert image: " << path << std::endl;
		return Engine::Core::TextureData{};
	}

	Engine::Core::TextureData textureData{};
	textureData.width = ilGetInteger(IL_IMAGE_WIDTH);
	textureData.height = ilGetInteger(IL_IMAGE_HEIGHT);
	textureData.channels = 4;
	textureData.name = name;

	ILubyte* data = ilGetData();
	
	if (data)
	{
		size_t numPixels = static_cast<size_t>(textureData.width) * textureData.height;
		size_t dataSize = numPixels * textureData.channels;
		textureData.pixels.assign(data, data + dataSize);
	}
	
	ilDeleteImages(1, &imageID);

	return textureData;
}

Engine::Core::MaterialData Engine::Infra::ImportFuncs::importMaterialData(const std::string& path, const std::string& name)
{
	std::ifstream materialFile(path);

	Core::MaterialData materialData{};

	if (!materialFile.is_open())
	{
		std::cerr << "Failed to open material file: " << path << std::endl;
		return Engine::Core::MaterialData{};
	}

	materialData.name = name;
	std::string line;

	auto stringToVec3 = [](const std::string& str) -> glm::vec3
		{
			std::istringstream iss(str);
			glm::vec3 vec;
			iss >> vec.x >> vec.y >> vec.z;
			return vec;
		};

	while (std::getline(materialFile, line))
	{
		std::stringstream ss(line);
		std::string firstWord;

		ss >> firstWord;

		if (firstWord == "Ns")
		{
			ss >> std::skipws;
			float ns;
			ss >> ns;
			materialData.ns = ns;
		}
		
		else if (firstWord == "Ni")
		{
			ss >> std::skipws;
			float ni;
			ss >> ni;
			materialData.ni = ni;
		}
		
		else if (firstWord == "d")
		{
			ss >> std::skipws;
			float d;
			ss >> d;
			materialData.d = d;
		}
		
		else if (firstWord == "illum")
		{
			ss >> std::skipws;
			int illum;
			ss >> illum;
			materialData.illum = illum;
		}
		
		else if (firstWord == "Ka")
		{
			std::string vecStr;
			std::getline(ss, vecStr);
			glm::vec3 ka = stringToVec3(vecStr);
			materialData.ka = ka;
		}
		
		else if (firstWord == "Kd")
		{
			std::string vecStr;
			std::getline(ss, vecStr);
			glm::vec3 kd = stringToVec3(vecStr);
			materialData.kd = kd;
		}
		
		else if (firstWord == "Ks")
		{
			std::string vecStr;
			std::getline(ss, vecStr);
			glm::vec3 ks = stringToVec3(vecStr);
			materialData.ks = ks;
		}
		
		else if (firstWord == "Ke")
		{
			std::string vecStr;
			std::getline(ss, vecStr);
			glm::vec3 ke = stringToVec3(vecStr);
			materialData.ke = ke;
		}

		else
		{
			std::string texturePath;
			ss >> texturePath;
			materialData.setMapFilePath(firstWord, texturePath);
		}
		/*
		else if (firstWord == "map_Kd")
		{
			std::string texturePath;
			ss >> texturePath;
			materialData.mapKdPath = texturePath;
		}
	
		else if (firstWord == "map_Ks")
		{
			std::string texturePath;
			ss >> texturePath;
			materialData.mapKsPath = texturePath;
		}
		
		else if (firstWord == "map_Ke")
		{
			std::string texturePath;
			ss >> texturePath;
			materialData.mapKePath = texturePath;
		}
		
		else if (firstWord == "map_Bump" || firstWord == "bump")
		{
			std::string texturePath;
			ss >> texturePath;
			materialData.mapBumpPath = texturePath;
		}*/
	}

	materialFile.close();
	return materialData;
}
