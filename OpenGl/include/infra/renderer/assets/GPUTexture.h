#pragma once
#include <GL/glew.h>
#include "core/assets/TextureData.h"
#include <memory>

namespace Engine::Infra
{
	class GpuTexture
	{
	public:
		GLuint id{ 0 };
		Core::TextureData* texture = nullptr;
		void genTexture()
		{
			if (texture->pixels.empty())
			{
				return;
			}

            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,            
                GL_RGBA8,     
                static_cast<GLsizei>(texture->width),
                static_cast<GLsizei>(texture->height),
                0,                  
                GL_RGBA,            
                GL_UNSIGNED_BYTE,   
                texture->pixels.data()  
            );

            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);

        }

		GpuTexture(Core::TextureData* _texture) : texture{ _texture } 
		{
		}

        ~GpuTexture()
        {
            if (id != 0) {
                glDeleteTextures(1, &id); // Cleanup VRAM leak
            }
        }

        void bind(GLuint slot = 0)
        {
            glActiveTexture(GL_TEXTURE0 + slot); // Select texture slot (GL_TEXTURE0, GL_TEXTURE1, etc.)
            glBindTexture(GL_TEXTURE_2D, id);
        }
	};
}