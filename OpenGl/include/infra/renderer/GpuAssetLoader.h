#pragma once
#include "core/assets/AssetManager.h"
#include "Renderer.h"
/*
void Engine::Infra::Renderer::loadMeshes(std::vector<Core::MeshData*>& meshes)
{
	int c = 0;
	for (const auto& mesh : meshes)
	{
		c++;
		mesh->recomputeNormals();

		cacheMesh(mesh);
		gpuMeshCache[mesh]->genBuffers();
	}

	screenQuad.create();
	std::cout << c << " meshes loaded!\n";
}

void Engine::Infra::Renderer::loadShaders(std::vector<Core::ShaderData*>& shaders)
{
	for (const auto& shader : shaders)
	{
		//std::cout << "shader name: " << shader->name << "\n";
		cacheShader(shader);
		if (shader->name == "lightDebugShader")
		{
			DebugLightShader = gpuShaderCache[shader].get();
		}
		else if (shader->name == "shadowMap")
		{
			shadowShader = gpuShaderCache[shader].get();
			std::cout << "shadow shader found!\n";
		}
		else if (shader->name == "depthBuffer")
		{
			depthShader = gpuShaderCache[shader].get();
			std::cout << "depth shader found!\n";
		}

		gpuShaderCache[shader]->compileShaders();
	}

}

void Engine::Infra::Renderer::loadTextures(std::vector<Core::TextureData*>& textures)
{
	for (const auto& texture : textures)
	{
		cacheTexture(texture);
		gpuTextureCache[texture]->genTexture();
	}
}
*/

namespace Engine::Infra
{
	class GpuAssetLoader
	{
	public:
		static void fillRenderer(Core::AssetManager& am, Renderer& renderer)
		{
			for (auto textureData : am.textureList())
			{
				std::string textureName = textureData->name;
				Core::TextureId id = am.getTextureId(textureName);

				renderer.cacheTexture(id, textureData);
				renderer.gpuTextureCache.get(id).get()->genTexture();
			}

			int c = 0;
			for (auto meshData : am.meshList())
			{
				c++;
				meshData->recomputeNormalsAndTangents();
				std::cout << c << " meshes loaded!\n";
				std::string meshName = meshData->name;
				Core::MeshId id = am.getMeshId(meshName);

				renderer.cacheMesh(id, meshData);
				renderer.gpuMeshCache.get(id)->genBuffers();
			}
			
			renderer.screenQuad.create();
			
			for (auto shaderData : am.shaderList())
			{
				std::string shaderName = shaderData->name;
				Core::ShaderId id = am.getShaderId(shaderName);

				renderer.cacheShader(id, shaderData);
				renderer.gpuShaderCache.get(id).get()->compileShaders();

				if (shaderName == "depthCubemap")
				{
					renderer.shadowCubemapShader = renderer.gpuShaderCache.get(id).get();
					std::cout << "depth cube map found!\n";
				}
			}
		}
	};


}