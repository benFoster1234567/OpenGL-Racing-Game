#include "infra/renderer/Renderer.h"
#include <memory>
#include <infra/renderer/assets/GPUMesh.h>
#include <infra/renderer/assets/GPUTexture.h>
#include <GL/glew.h>
#include <infra/renderer/assets/GpuShader.h>

#include <glm/gtc/type_ptr.hpp>

void Engine::Infra::Renderer::cacheMesh(Core::MeshData* meshData)
{
	auto gpuMesh = std::make_unique<GpuMesh>(meshData);
	gpuMeshCache.emplace(meshData, std::move(gpuMesh));
}

void Engine::Infra::Renderer::drawLights(Core::ShaderData* lightShader, size_t lightCount)
{
	GpuShader* gpuShader = gpuShaderCache[lightShader].get();
	
	glBindVertexArray(emptyVao);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);

	glDrawArrays(GL_POINTS, 0, lightCount);

}

void Engine::Infra::Renderer::cacheShader(Core::ShaderData* shaderData)
{
	auto gpuShader = std::make_unique<GpuShader>(shaderData);

	gpuShaderCache.emplace(shaderData, std::move(gpuShader));
}

void Engine::Infra::Renderer::cacheTexture(Core::TextureData* textureData)
{

	auto gpuTexture = std::make_unique<GpuTexture>(textureData);
	gpuTextureCache.emplace(textureData, std::move(gpuTexture));
}

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

void Engine::Infra::Renderer::loadLights(std::vector<StaticPointLightResource>& staticLights)
{
	glEnable(GL_PROGRAM_POINT_SIZE);
	//must be loaded after shaders?
	StaticPointLight lights[MAX_LIGHTS];

	glGenVertexArrays(1, &emptyVao);

	size_t lightsToCopy = (std::min)(staticLights.size(), static_cast<size_t>(MAX_LIGHTS));
	for (size_t i = 0; i < lightsToCopy; ++i) {
		auto lightCpu = staticLights[i];
		StaticPointLight lightGpu
		{
			.posRad = {lightCpu.position, lightCpu.radius},
			.color = {lightCpu.color, 1.0f}
		};

		lights[i] = lightGpu;
	}

	UboStaticPointLightData uboData{};

	std::memcpy(uboData.lights, lights, sizeof(lights));
	uboData.activeLightCount = static_cast<int>(lightsToCopy);

	activeLightCount = uboData.activeLightCount;

	//GLuint ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);

	glBufferData(GL_UNIFORM_BUFFER, sizeof(UboStaticPointLightData), &uboData, GL_STATIC_DRAW);

	GLuint bindingPoint{ 0 };
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	for (const auto& [shaderData, shader] : gpuShaderCache)
	{
		GLuint blockIndex = glGetUniformBlockIndex(shader->Id, "LightBlock");

		if (blockIndex != GL_INVALID_INDEX)
		{
			glUniformBlockBinding(shader->Id, blockIndex, bindingPoint);
		}
	}
}

void Engine::Infra::Renderer::renderLights()
{
	glUseProgram(DebugLightShader->getId());
	glBindVertexArray(emptyVao);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
	glDrawArrays(GL_POINTS, 0 , activeLightCount);

}

void Engine::Infra::Renderer::submit(RenderCommand command)
{
	if (!gpuMeshCache.contains(command.mesh))
	{
		std::cerr << "no mesh exists on the gpu with name: " << command.mesh->name << "\nMesh needs to be submitted at the start of the program";
		exit(1);
	}

	renderQueue.push_back(command);
}

void Engine::Infra::Renderer::flush()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode == LINE? GL_LINE : GL_FILL);

	for (const auto& command : renderQueue)
	{

		if (!command.material) {
			std::cerr << "Render command has a null material pointer!\n";
			continue;
		}

		if (!gpuMeshCache.contains(command.mesh))
		{
			std::cerr << "no mesh exists on the gpu with name: " << command.mesh->name << "\nMesh needs to be submitted at the start of the program";
			exit(1);
		}

		if (!gpuShaderCache.contains(command.shader))
		{
			std::cerr << "no shader exists on the gpu with name: " << command.shader->name << "\nShader needs to be submitted at the start of the program";
			exit(1);
		}

		if (command.shader == nullptr)
		{
			throw std::runtime_error("shader is null");
		}

		GpuMesh* mesh = gpuMeshCache[command.mesh].get();
		GpuShader* shader = gpuShaderCache[command.shader].get();

		GpuTexture* ambient		= gpuTextureCache[command.material->mapTextures[int(Core::MaterialData::MapType::Ambient)]].get();
		GpuTexture* diffuse		= gpuTextureCache[command.material->mapTextures[int(Core::MaterialData::MapType::Diffuse)]].get();
		GpuTexture* specular	= gpuTextureCache[command.material->mapTextures[int(Core::MaterialData::MapType::Specular)]].get();
		GpuTexture* normal		= gpuTextureCache[command.material->mapTextures[int(Core::MaterialData::MapType::Normal)]].get();
		
		if (diffuse == nullptr)
		{
			throw std::runtime_error("diffuse is null");
		}

		if (ambient == nullptr)
		{
			throw std::runtime_error("ambient is null: " + command.material->name);
		}

		glUseProgram(shader->getId());

		auto projectionMatrixLocation	= glGetUniformLocation(shader->getId(), "projection");
		auto viewMatrixLocation			= glGetUniformLocation(shader->getId(), "view");
		auto modelMatrixLocation		= glGetUniformLocation(shader->getId(), "model");
		
		assert(!(projectionMatrixLocation == -1 || viewMatrixLocation == -1 || modelMatrixLocation == -1) && "error sending mvp to shader");

		auto ambientLocation	= glGetUniformLocation(shader->getId(), "material.ambient");
		auto diffuseLocation	= glGetUniformLocation(shader->getId(), "material.diffuse");
		auto normalLocation		= glGetUniformLocation(shader->getId(), "material.normal");
		auto specularLocation	= glGetUniformLocation(shader->getId(), "material.specular");
		auto shininessLocation	= glGetUniformLocation(shader->getId(), "material.shininess");
		
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, ambient->id);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, diffuse->id);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, normal->id);
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, specular->id);
		
		glUniform1i(ambientLocation, 0);
		glUniform1i(diffuseLocation, 1);
		glUniform1i(normalLocation, 2);
		glUniform1i(specularLocation, 3);
		glUniform1f(shininessLocation, command.material->ns);

		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(command.projection));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(command.view));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(command.modelTransform));

		diffuse->bind();

		mesh->draw();
	}

	renderQueue.clear();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}
