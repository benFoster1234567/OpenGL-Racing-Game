#pragma once
#include <GL/glew.h>
#include "Texture.h"
#include <memory>

class GpuTexture
{
private:
public:
	std::shared_ptr<Texture> texture;
	GLuint id;
	void bind();
};