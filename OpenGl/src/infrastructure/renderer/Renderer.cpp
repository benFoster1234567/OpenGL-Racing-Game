#include "infra/renderer/Renderer.h"

void Engine::Infra::Renderer::sendMaterialToShader(Core::MaterialInfo m, GLuint shaderId)
{
	
}

void Engine::Infra::Renderer::sendTransformToShader(Core::Transform* t, GLuint shaderId)
{

}

void Engine::Infra::Renderer::clearColor()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Engine::Infra::Renderer::submit(Core::Entity entity)
{
	auto meshIt = gpuMeshes.find(entity.mesh);
	if (meshIt == gpuMeshes.end())
	{
		gpuMeshes.emplace(entity.mesh, GpuMesh(entity.mesh));
		gpuMeshes[entity.mesh].genBuffers();
	}

	auto diffuseIt = gpuTextures.find(entity.material->diffuse);
	if (diffuseIt == gpuTextures.end())
		gpuTextures[entity.material->diffuse] = GpuTexture(entity.material->diffuse);

	auto normalIt = gpuTextures.find(entity.material->normal);
	if (normalIt == gpuTextures.end())
		gpuTextures[entity.material->normal] = GpuTexture(entity.material->normal);

	RenderCommand cmd
	{
		.diffuse = &gpuTextures[entity.material->diffuse],
		.normal = &gpuTextures[entity.material->normal],
		.mesh = &gpuMeshes[entity.mesh],
		.material = entity.material,
		.transform = &entity.transform
	};
	renderQueue.push_back(cmd);
}

void Engine::Infra::Renderer::flush()
{

}
