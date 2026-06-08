#include "infra/renderer/Renderer.h"
#include <memory>
#include <core/ecs/Entity.h>
#include <infra/renderer/assets/GPUMesh.h>
#include <infra/renderer/assets/GPUTexture.h>
#include <GL/glew.h>
#include <infra/renderer/assets/GpuShader.h>


void Engine::Infra::Renderer::executeDraw(RenderCommand cmd)
{
	auto transform = cmd.transform->pos * cmd.transform->rot * cmd.transform->rot;
	glUniformMatrix4fv(glGetUniformLocation(cmd.shader->getId(), "Transform"), 1, GL_FALSE, (GLfloat*)&transform[0]);
	glUniform4fv(glGetUniformLocation(cmd.shader->getId(), "Specular"), 1, (GLfloat*)&cmd.material->materialSettings.specular[0]);
	glUniform4fv(glGetUniformLocation(cmd.shader->getId(), "Ambient"), 1, (GLfloat*)&cmd.material->materialSettings.ambient[0]);
	glUniform4fv(glGetUniformLocation(cmd.shader->getId(), "Diffuse"), 1, (GLfloat*)&cmd.material->materialSettings.diffuse[0]);
	cmd.mesh->draw();
}

void Engine::Infra::Renderer::clearColor()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Engine::Infra::Renderer::submit(Core::Entity entity)
{
	//TODO: put this in a new setup method for importing assets

	if (!gpuMeshes.contains(entity.mesh))
		gpuMeshes.emplace(entity.mesh, std::make_unique<GpuMesh>(entity.mesh));

	if (!gpuTextures.contains(entity.material->diffuse))
		gpuTextures.emplace(entity.material->diffuse, std::make_unique<GpuTexture>(entity.material->diffuse));

	if (!gpuTextures.contains(entity.material->normal))
		gpuTextures.emplace(entity.material->normal, std::make_unique<GpuTexture>(entity.material->normal));

	RenderCommand cmd
	{
		.mesh = gpuMeshes[entity.mesh].get(),
		.diffuse = gpuTextures[entity.material->diffuse].get(),
		.normal = gpuTextures[entity.material->normal].get(),
		.shader = gpuShaders[entity.material->shader].get(),
		.material = entity.material,
		.transform = &entity.transform
	};
	renderQueue.push_back(cmd);
}

void Engine::Infra::Renderer::flush()
{
	for (const auto& c : renderQueue)
	{
		executeDraw(c);
	}
	renderQueue.clear();
}
