#pragma once
#include <GL/glew.h>
#include "core/assets/TextureData.h"
#include <memory>

namespace Engine::Infra
{
	class GpuTexture
	{
	private:
	public:
		Core::TextureData* texture;
		GLuint id;
		void bind();
	};
}