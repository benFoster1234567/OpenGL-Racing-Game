#include "infra/renderer/Renderer.h"
#include <memory>
#include <infra/renderer/assets/GPUMesh.h>
#include <infra/renderer/assets/GPUTexture.h>
#include <GL/glew.h>
#include <infra/renderer/assets/GpuShader.h>

#include <glm/gtc/type_ptr.hpp>

void Engine::Infra::Renderer::cacheMesh(Core::MeshId meshId, Core::MeshData* meshData)
{
	auto gpuMesh = std::make_unique<GpuMesh>(meshData);
	gpuMeshCache.insert(meshId, std::move(gpuMesh));
}

void Engine::Infra::Renderer::drawLights(Core::ShaderId shaderId, size_t lightCount)
{
	GpuShader* gpuShader = gpuShaderCache.get(shaderId).get();
	
	glBindVertexArray(emptyVao);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);

	glDrawArrays(GL_POINTS, 0, lightCount);

}

void Engine::Infra::Renderer::cacheShader(Core::ShaderId shaderId, Core::ShaderData* shaderData)
{
	auto gpuShader = std::make_unique<GpuShader>(shaderData);

	// Ensure newly cached shaders link their LightBlock uniform to UBO binding point 0
	GLuint blockIndex = glGetUniformBlockIndex(gpuShader->getId(), "LightBlock");
	if (blockIndex != GL_INVALID_INDEX)
	{
		glUniformBlockBinding(gpuShader->getId(), blockIndex, 0);
	}

	gpuShaderCache.insert(shaderId, std::move(gpuShader));
}

void Engine::Infra::Renderer::cacheTexture(Core::TextureId textureId, Core::TextureData* textureData)
{
	auto gpuTexture = std::make_unique<GpuTexture>(textureData);
	gpuTextureCache.insert(textureId, std::move(gpuTexture));
}

void Engine::Infra::Renderer::loadLights(std::vector<StaticPointLightResource>& staticLights)
{
	staticPointLights = staticLights;
	glEnable(GL_PROGRAM_POINT_SIZE);
	StaticPointLight lights[MAX_LIGHTS];

	glGenVertexArrays(1, &emptyVao);

	size_t lightsToCopy = (std::min)(staticLights.size(), static_cast<size_t>(MAX_LIGHTS));
	for (size_t i = 0; i < lightsToCopy; ++i) {
		auto lightCpu = staticLights[i];
		StaticPointLight lightGpu
		{
			.posRad = {lightCpu.position, lightCpu.radius},
			.color = {lightCpu.color, lightCpu.intensity}
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

	for (const auto& shader : gpuShaderCache)
	{
		GLuint blockIndex = glGetUniformBlockIndex(shader->getId(), "LightBlock");

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
		std::cerr << "no mesh exists on the gpu with id: " << command.mesh << "\nMesh needs to be submitted at the start of the program";
		exit(1);
	}

	renderQueue.push_back(command);
}



void Engine::Infra::Renderer::flush(size_t w , size_t h)
{
	shadowCubemapShader->use();
	//glCullFace(GL_FRONT);
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	GLuint shadowShaderId = shadowCubemapShader->getId();
	glUniformMatrix4fv(glGetUniformLocation(shadowShaderId, "shadowMatrices"), 6, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
	glUniform3fv(glGetUniformLocation(shadowShaderId, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform1f(glGetUniformLocation(shadowShaderId, "far_plane"), ffar);

	for (const auto& command : renderQueue)
	{
		GpuMesh* mesh = gpuMeshCache.get(command.mesh).get();
		glUniformMatrix4fv(glGetUniformLocation(shadowShaderId, "model"), 1, GL_FALSE, glm::value_ptr(command.modelTransform));
		mesh->draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, w, h);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);

	for (const auto& command : renderQueue)
	{
		if (!command.material) continue;

		GpuMesh* mesh = gpuMeshCache.get(command.mesh).get();
		GpuShader* shader = gpuShaderCache.get(command.shader).get();

		GpuTexture* ambient = gpuTextureCache.get(command.material->mapTextures[int(Core::MaterialData::MapType::Ambient)]).get();
		GpuTexture* diffuse = gpuTextureCache.get(command.material->mapTextures[int(Core::MaterialData::MapType::Diffuse)]).get();
		GpuTexture* specular = gpuTextureCache.get(command.material->mapTextures[int(Core::MaterialData::MapType::Specular)]).get();
		GpuTexture* normal = gpuTextureCache.get(command.material->mapTextures[int(Core::MaterialData::MapType::Normal)]).get();

		glUseProgram(shader->getId());
		glUniform1f(glGetUniformLocation(shader->getId(), "far_plane"), ffar);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemapId);

		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, ambient->id);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, diffuse->id);
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, normal->id);
		glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, specular->id);

		glUniform1i(glGetUniformLocation(shader->getId(), "depthMap"), 0);
		glUniform1i(glGetUniformLocation(shader->getId(), "material.ambient"), 1);
		glUniform1i(glGetUniformLocation(shader->getId(), "material.diffuse"), 2);
		glUniform1i(glGetUniformLocation(shader->getId(), "material.normal"), 3);
		glUniform1i(glGetUniformLocation(shader->getId(), "material.specular"), 4);
		glUniform1f(glGetUniformLocation(shader->getId(), "material.shininess"), command.material->ns);

		glUniformMatrix4fv(glGetUniformLocation(shader->getId(), "projection"), 1, GL_FALSE, glm::value_ptr(command.projection));
		glUniformMatrix4fv(glGetUniformLocation(shader->getId(), "view"), 1, GL_FALSE, glm::value_ptr(command.view));
		glUniformMatrix4fv(glGetUniformLocation(shader->getId(), "model"), 1, GL_FALSE, glm::value_ptr(command.modelTransform));

		glm::vec3 camPos = glm::vec3(glm::inverse(command.view)[3]);
		glUniform3fv(glGetUniformLocation(shader->getId(), "viewPos"), 1, glm::value_ptr(camPos));

		mesh->draw();
	}

	renderQueue.clear();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}
