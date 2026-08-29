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

	pointlightLoader.bindLightBufferBase();

	glDrawArrays(GL_POINTS, 0, lightCount);

}

std::vector<glm::mat4> Engine::Infra::Renderer::getTransformCubemapArray(std::vector<glm::vec3> lightPositions)
{
	std::vector<glm::mat4> transforms{};

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;

	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, nnear, ffar);

	for (int i{}; i < 4; i++)
	{
		glm::vec3 lp{ 0.0f };
		if (i < staticPointLights.size())
		{
			lp = staticPointLights[i].position;
		}

		transforms.push_back(shadowProj * glm::lookAt(lp, lp + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		transforms.push_back(shadowProj * glm::lookAt(lp, lp + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		transforms.push_back(shadowProj * glm::lookAt(lp, lp + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		transforms.push_back(shadowProj * glm::lookAt(lp, lp + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		transforms.push_back(shadowProj * glm::lookAt(lp, lp + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		transforms.push_back(shadowProj * glm::lookAt(lp, lp + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
	}

	return transforms;
}

void Engine::Infra::Renderer::prepareDepthCubemapArray()
{
	if (staticPointLights.empty()) return;


	///pointlightLoader.loadShadowCastedPointlights(staticPointLights, nnear, ffar);

	for (const auto& shader : gpuShaderCache)
	{
		pointlightLoader.bindShadowBlockToShader(shader->getId());
	}

	if (depthMapFBO == 0)
	{
		glGenFramebuffers(1, &depthMapFBO);
		glGenTextures(1, &depthCubemapId);

		glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, depthCubemapId);
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		GLsizei totalLayers = static_cast<GLsizei>(staticPointLights.size() * 6);

		glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY
			, 0
			, GL_DEPTH_COMPONENT24
			, SHADOW_WIDTH
			, SHADOW_HEIGHT
			, totalLayers
			, 0
			, GL_DEPTH_COMPONENT
			, GL_FLOAT
			, NULL
		);

		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemapId, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


	}
	

	
}

void Engine::Infra::Renderer::prepareDepthCubemap()
{
	if (staticPointLights.empty()) return;

	lightPos = staticPointLights[0].position;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	if (depthMapFBO == 0)
	{
		glGenFramebuffers(1, &depthMapFBO);
		glGenTextures(1, &depthCubemapId);

		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemapId);

		GLsizei totalLayers = static_cast<GLsizei>(staticPointLights.size() * 6);

		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemapId, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


	}

	// Recalculate shadow matrices
	shadowTransforms.clear();
	float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
	nnear = 1.0f;
	ffar = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, nnear, ffar);

}

void Engine::Infra::Renderer::renderToShadowCubemapArray(size_t w, size_t h)
{
	shadowCubemapShader->use();
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	GLuint shadowShaderId = shadowCubemapShader->getId();

	glUniform1f(glGetUniformLocation(shadowShaderId, "far_plane"), ffar);

	for (const auto& command : renderQueue)
	{
		GpuMesh* mesh = gpuMeshCache.get(command.mesh).get();
		glUniformMatrix4fv(glGetUniformLocation(shadowShaderId, "model"), 1, GL_FALSE, glm::value_ptr(command.modelTransform));
		mesh->draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

void Engine::Infra::Renderer::renderToShadowCubemap(size_t w, size_t h)
{
	//glEnable(GL_DEPTH_TEST);
	shadowCubemapShader->use();
	//glCullFace(GL_FRONT);
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	GLuint shadowShaderId = shadowCubemapShader->getId();

	glUniformMatrix4fv(glGetUniformLocation(shadowShaderId, "shadowMatrices"), 6, GL_FALSE, glm::value_ptr(getTransforms(lightPos)[0]));
	glUniform3fv(glGetUniformLocation(shadowShaderId, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform1f(glGetUniformLocation(shadowShaderId, "far_plane"), ffar);

	for (const auto& command : renderQueue)
	{
		GpuMesh* mesh = gpuMeshCache.get(command.mesh).get();
		glUniformMatrix4fv(glGetUniformLocation(shadowShaderId, "model"), 1, GL_FALSE, glm::value_ptr(command.modelTransform));
		mesh->draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::Infra::Renderer::loadLights(std::vector<StaticPointLightResource> staticLights)
{
	staticPointLights = staticLights;

	glGenVertexArrays(1, &emptyVao);
	pointlightLoader.loadStaticPointlights(staticLights);
	activeLightCount = pointlightLoader.getActiveLightCount();

	for (const auto& shader : gpuShaderCache)
	{
		pointlightLoader.bindLightBlockToShader(shader->getId());
	}
}

void Engine::Infra::Renderer::loadShadowingLights(std::vector<StaticPointLightResource> staticLights)
{
	pointlightLoader.loadShadowCastedPointlights(staticLights, nnear, ffar);
}

void Engine::Infra::Renderer::renderLights()
{
	glUseProgram(DebugLightShader->getId());
	glBindVertexArray(emptyVao);
	pointlightLoader.bindLightBufferBase();

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
	renderToShadowCubemapArray(w,h);

	glViewport(0, 0, w, h);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pointlightLoader.bindLightBufferBase();

	bool renderMultiLightShadows = true;

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
		if (renderMultiLightShadows)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, depthCubemapId);
		}

		else
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemapId);
		}

		glUniform2fv(glGetUniformLocation(shader->getId(), "uvScale"), 1, glm::value_ptr(command.uvScale));

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
