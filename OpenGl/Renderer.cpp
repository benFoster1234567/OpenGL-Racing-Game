#include "Renderer.h"

void Renderer::sendMaterialToShader(MaterialInfo m, GLuint shaderId)
{
	glUniform4fv(glGetUniformLocation(shaderId, "Ambient"), 1, (GLfloat*)&m.ambient[0]);
	glUniform4fv(glGetUniformLocation(shaderId, "Diffuse"), 1, (GLfloat*)&m.diffuse[0]);
	glUniform4fv(glGetUniformLocation(shaderId, "Specular"), 1, (GLfloat*)&m.specular[0]);
}

void Renderer::sendTransformToShader(Transform* t, GLuint shaderId)
{
	auto modelMatrix = t->pos * t->rot * t->scale;
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "Model"), 1, GL_FALSE, (GLfloat*)&modelMatrix[0]);
}

void Renderer::submit(Entity entity)
{
	renderQueue.push_back(RenderCommand{
		.shader = &shaders[entity.shaderFileName],
		.mesh = &gpuMeshes[entity.mesh],
		.diffuse = &gpuTextures[entity.material->diffuse],
		.normal = &gpuTextures[entity.material->normal],
		.material = entity.material->shaderSettings,
		.transform = &entity.transform
		});
}

void Renderer::flush()
{
	if (renderQueue.empty()) return;
	for (const auto& cmd : renderQueue)
	{
		cmd.shader->use();
		cmd.diffuse->bind();
		
		sendMaterialToShader(cmd.material, cmd.shader->getId());
		sendTransformToShader(cmd.transform, cmd.shader->getId());
	}
}
