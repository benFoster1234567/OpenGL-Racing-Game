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
	//must be loaded after shaders?
	StaticPointLight lights[MAX_LIGHTS];

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

	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);

	glBufferData(GL_UNIFORM_BUFFER, sizeof(UboStaticPointLightData), &uboData, GL_STATIC_DRAW);

	GLuint bindingPoint{ 0 };
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, UBO);
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

		GpuMesh* mesh = gpuMeshCache[command.mesh].get();
		GpuShader* shader = gpuShaderCache[command.shader].get();
		glUseProgram(shader->getId());

		auto p = glGetUniformLocation(shader->getId(), "projection");
		auto v = glGetUniformLocation(shader->getId(), "view");
		auto m = glGetUniformLocation(shader->getId(), "model");

		auto d = glGetUniformLocation(shader->getId(), "material.diffuse");
		auto a = glGetUniformLocation(shader->getId(), "material.ambient");
		auto s = glGetUniformLocation(shader->getId(), "material.specular");
		auto k = glGetUniformLocation(shader->getId(), "material.shininess");
		auto e = glGetUniformLocation(shader->getId(), "material.emission");

		assert(!(p == -1 || v == -1 || m == -1) && "error sending mvp to shader");

		glUniform3fv(d, 1, glm::value_ptr(command.material->kd));
		glUniform3fv(a, 1, glm::value_ptr(command.material->ka));
		glUniform3fv(s, 1, glm::value_ptr(command.material->ks));
		glUniform3fv(e, 1, glm::value_ptr(command.material->ke));
		glUniform1f(k, command.material->ns);

		glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(command.projection));
		glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(command.view));
		glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(command.modelTransform));

		mesh->draw();
	}

	renderQueue.clear();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}
