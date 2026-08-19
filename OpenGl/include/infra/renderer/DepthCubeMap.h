#pragma once
#include <GL/glew.h>
namespace Engine::Infra
{
	class DepthCubeMap
	{
	private:
		GLuint depthCubemap	{ 0 };
		size_t shadowWidth	{ 0 };
		size_t shadowHeight	{ 0 };

	public:
		DepthCubeMap(size_t width, size_t height) : shadowWidth{ width }, shadowHeight{ height }
		{
		}

		void generate()
		{
			glGenTextures(1, &depthCubemap);
		}

		void bind()
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		}

		void prepareCubemapFaces()
		{
			for (unsigned int i = 0; i < 6; ++i)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
					shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (void*)0);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,	 GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,	 GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,		 GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,		 GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,		 GL_CLAMP_TO_EDGE);
		}

		void setViewport()
		{
			glViewport(0, 0, shadowWidth, shadowHeight);
		}

		void attach()	{ glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0); }
		void destroy()	{ glDeleteTextures(1, &depthCubemap); depthCubemap = 0; }

	};
}