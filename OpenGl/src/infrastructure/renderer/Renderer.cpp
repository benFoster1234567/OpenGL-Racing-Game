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
}

void Engine::Infra::Renderer::flush()
{
}
