#pragma once
#include <GL/glew.h>
#include "core/assets/TextureData.h"
#include <memory>

class GpuTexture
{
private:
public:
	TextureData* texture;
	GLuint id;
	void bind();
};