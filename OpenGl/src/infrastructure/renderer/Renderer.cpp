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
	//must be loaded after shaders?
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

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	shadowShader->use();
	shadowMap.bindFramebuffer();
	glViewport(0, 0, shadowMap.getWidth(), shadowMap.getHeight());
	glClear(GL_DEPTH_BUFFER_BIT);

	StaticPointLightResource light = staticPointLights[0];
	for (const auto& command : renderQueue)
	{
		configureShadowShadersAndMatrices(command, light);
		gpuMeshCache.get(command.mesh)->draw();
	}
	shadowMap.unbindFrameBuffer();

	glDisable(GL_CULL_FACE); 
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, w, h);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (renderMode)
	{
	case 0:
		for (const auto& command : renderQueue)
		{

			if (!command.material) {
				std::cerr << "Render command has a null material pointer!\n";
				continue;
			}

			if (!gpuMeshCache.contains(command.mesh))
			{
				std::cerr << "no mesh exists on the gpu with id: " << command.mesh << "\nMesh needs to be submitted at the start of the program";
				exit(1);
			}

			if (!gpuShaderCache.contains(command.shader))
			{
				std::cerr << "no shader exists on the gpu with id: " << command.shader << "\nShader needs to be submitted at the start of the program";
				exit(1);
			}

			GpuMesh* mesh = gpuMeshCache.get(command.mesh).get();
			GpuShader* shader = gpuShaderCache.get(command.shader).get();

			if (mesh == nullptr)
			{
				throw std::runtime_error("mesh is null");
			}

			GpuTexture* ambient = gpuTextureCache.get(command.material->mapTextures[int(Core::MaterialData::MapType::Ambient)]).get();
			GpuTexture* diffuse = gpuTextureCache.get(command.material->mapTextures[int(Core::MaterialData::MapType::Diffuse)]).get();
			GpuTexture* specular = gpuTextureCache.get(command.material->mapTextures[int(Core::MaterialData::MapType::Specular)]).get();
			GpuTexture* normal = gpuTextureCache.get(command.material->mapTextures[int(Core::MaterialData::MapType::Normal)]).get();

			if (diffuse == nullptr)
			{
				throw std::runtime_error("diffuse is null");
			}

			if (ambient == nullptr)
			{
				throw std::runtime_error("ambient is null: " + command.material->name);
			}

			glUseProgram(shader->getId());

			auto projectionMatrixLocation = glGetUniformLocation(shader->getId(), "projection");
			auto viewMatrixLocation = glGetUniformLocation(shader->getId(), "view");
			auto modelMatrixLocation = glGetUniformLocation(shader->getId(), "model");

			assert(!(projectionMatrixLocation == -1 || viewMatrixLocation == -1 || modelMatrixLocation == -1) && "error sending mvp to shader");
			
			auto shadowMapLocation = glGetUniformLocation(shader->getId(), "shadowMap");
			auto ambientLocation = glGetUniformLocation(shader->getId(), "material.ambient");
			auto diffuseLocation = glGetUniformLocation(shader->getId(), "material.diffuse");
			auto normalLocation = glGetUniformLocation(shader->getId(), "material.normal");
			auto specularLocation = glGetUniformLocation(shader->getId(), "material.specular");
			auto shininessLocation = glGetUniformLocation(shader->getId(), "material.shininess");

			shadowMap.bindDepthMap();
			glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, ambient->id);
			glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, diffuse->id);
			glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, normal->id);
			glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, specular->id);

			glUniform1i(shadowMapLocation, 0);
			glUniform1i(ambientLocation, 1);
			glUniform1i(diffuseLocation, 2);
			glUniform1i(normalLocation, 3);
			glUniform1i(specularLocation, 4);

			glUniform1f(shininessLocation, command.material->ns);

			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(command.projection));
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(command.view));
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(command.modelTransform));
			glUniformMatrix4fv(glGetUniformLocation(shader->getId(), "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(getLightSpaceMatrix(command, light)));
			mesh->draw();
		}
		break;
	case 1:
		depthShader->use();

		GLint location = glGetUniformLocation(depthShader->getId(), "depthMap");
		if (location != -1)
		{
			glUniform1i(location, 0);
		}

		glActiveTexture(GL_TEXTURE0);
		shadowMap.bindDepthMap();

		screenQuad.render(depthShader->getId());
		break;
	}

	
	renderQueue.clear();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}
