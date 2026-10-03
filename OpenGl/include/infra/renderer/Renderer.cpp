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
	m_gpuMeshCache.insert(meshId, std::move(gpuMesh));
}

void Engine::Infra::Renderer::drawLights(Core::ShaderId shaderId, size_t lightCount)
{
	GpuShader* gpuShader = m_gpuShaderCache.get(shaderId).get();

	glBindVertexArray(m_emptyVao);

	pointlightLoader.bindLightBufferBase();

	glDrawArrays(GL_POINTS, 0, lightCount);

}

void Engine::Infra::Renderer::prepareDepthCubemapArray()
{
	if (m_staticPointLights.empty()) return;


	///pointlightLoader.loadShadowCastedPointlights(staticPointLights, nnear, ffar);

	for (const auto& shader : m_gpuShaderCache)
	{
		pointlightLoader.bindShadowBlockToShader(shader->getId());
	}

	if (m_depthMapFbo == 0)
	{
		glGenFramebuffers(1, &m_depthMapFbo);
		glGenTextures(1, &m_depthCubemapId);

		glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_depthCubemapId);
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		GLsizei totalLayers = static_cast<GLsizei>(m_staticPointLights.size() * 6);

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

		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFbo);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthCubemapId, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


	}



}

void Engine::Infra::Renderer::prepareDepthCubemap()
{
	if (m_staticPointLights.empty()) return;

	m_lightPos = m_staticPointLights[0].position;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	if (m_depthMapFbo == 0)
	{
		glGenFramebuffers(1, &m_depthMapFbo);
		glGenTextures(1, &m_depthCubemapId);

		glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthCubemapId);

		GLsizei totalLayers = static_cast<GLsizei>(m_staticPointLights.size() * 6);

		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFbo);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthCubemapId, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


	}

	// Recalculate shadow matrices
	m_shadowTransforms.clear();
	float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
	m_near = 1.0f;
	m_far = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, m_near, m_far);

}

void Engine::Infra::Renderer::renderToShadowCubemapArray(size_t w, size_t h)
{
	glCullFace(GL_FRONT);
	m_shadowCubemapShader->use();
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	GLuint shadowShaderId = m_shadowCubemapShader->getId();

	glUniform1f(glGetUniformLocation(shadowShaderId, "far_plane"), m_far);

	for (const auto& command : m_renderQueue)
	{
		GpuMesh* mesh = m_gpuMeshCache.get(command.mesh).get();
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

	m_gpuShaderCache.insert(shaderId, std::move(gpuShader));
}

void Engine::Infra::Renderer::cacheTexture(Core::TextureId textureId, Core::TextureData* textureData)
{
	auto gpuTexture = std::make_unique<GpuTexture>(textureData);
	m_gpuTextureCache.insert(textureId, std::move(gpuTexture));
}

void Engine::Infra::Renderer::loadLights(std::vector<StaticPointLightResource> staticLights)
{
	m_staticPointLights = staticLights;

	glGenVertexArrays(1, &m_emptyVao);
	pointlightLoader.loadStaticPointlights(staticLights);
	m_activeLightCount = pointlightLoader.getActiveLightCount();

	for (const auto& shader : m_gpuShaderCache)
	{
		pointlightLoader.bindLightBlockToShader(shader->getId());
	}
}

void Engine::Infra::Renderer::loadShadowingLights(glm::vec3 cameraOrigin)
{
	pointlightLoader.loadPointShadowSources(cameraOrigin, m_near, m_far);
	for (const auto& shader : m_gpuShaderCache)
	{
		pointlightLoader.bindShadowBlockToShader(shader->getId());
	}
}



void Engine::Infra::Renderer::submit(RenderCommand command)
{
	if (!m_gpuMeshCache.contains(command.mesh))
	{
		std::cerr << "no mesh exists on the gpu with id: " << command.mesh << "\nMesh needs to be submitted at the start of the program";
		exit(1);
	}

	m_renderQueue.push_back(command);
}

void Engine::Infra::Renderer::flush(size_t w, size_t h)
{
	renderToShadowCubemapArray(w, h);

	glViewport(0, 0, w, h);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pointlightLoader.bindLightBufferBase();
	pointlightLoader.bindShadowBufferBase();
	bool renderMultiLightShadows = true;

	for (const auto& command : m_renderQueue)
	{
		if (!command.material) continue;

		GpuMesh* mesh = m_gpuMeshCache.get(command.mesh).get();
		GpuShader* shader = m_gpuShaderCache.get(command.shader).get();

		GpuTexture* ambient = m_gpuTextureCache.get(command.material->mapTextures[int(Core::MaterialData::MapType::Ambient)]).get();
		GpuTexture* diffuse = m_gpuTextureCache.get(command.material->mapTextures[int(Core::MaterialData::MapType::Diffuse)]).get();
		GpuTexture* specular = m_gpuTextureCache.get(command.material->mapTextures[int(Core::MaterialData::MapType::Specular)]).get();
		GpuTexture* normal = m_gpuTextureCache.get(command.material->mapTextures[int(Core::MaterialData::MapType::Normal)]).get();

		glUseProgram(shader->getId());
		glUniform1f(glGetUniformLocation(shader->getId(), "far_plane"), m_far);

		glActiveTexture(GL_TEXTURE0);
		if (renderMultiLightShadows)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_depthCubemapId);
		}

		else
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthCubemapId);
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

	m_renderQueue.clear();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}
