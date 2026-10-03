#pragma once
#include <GL/glew.h>

namespace Engine::Infra
{
	class Framebuffer
	{
	private:
		GLuint m_fbo{ 0 };
		bool m_initialized{ false };


	public:
		void generateFbo()
		{
			if (!m_initialized)
			{
				m_initialized = true;
				glGenFramebuffers(1, &m_fbo);
			}
		}

		void destroyFbo()
		{
			if (m_initialized)
			{
				glDeleteFramebuffers(1, &m_fbo);
				m_initialized = false;
				m_fbo = 0;
			}
		}
		Framebuffer() {}
		~Framebuffer() { destroyFbo(); }

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;

		Framebuffer(Framebuffer&& other) noexcept
			: m_fbo(other.m_fbo), m_initialized(other.m_initialized)
		{
			other.m_fbo = 0;
			other.m_initialized = false;
		}

		Framebuffer& operator=(Framebuffer&& other) noexcept
		{
			if (this != &other)
			{
				destroyFbo();
				m_fbo = other.m_fbo;
				m_initialized = other.m_initialized;
				other.m_fbo = 0;
				other.m_initialized = false;
			}
			return *this;
		}

		void bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); }
		void bindRead() { glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo); }
		void bindDraw() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo); }
		void clearDrawBuffer() { glDrawBuffer(GL_NONE); }
		void clearReadBuffer() { glReadBuffer(GL_NONE); }
		void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

		bool complete() { return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE; }
	};
}