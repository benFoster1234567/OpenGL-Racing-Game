#pragma once
#include <GL/glew.h>

namespace Engine::Infra
{
    class Framebuffer
    {
    private:
        GLuint fbo{ 0 };
        bool initialized{ false };


    public:
        void generateFbo()
        {
            if (!initialized)
            {
                initialized = true;
                glGenFramebuffers(1, &fbo);
            }
        }

        void destroyFbo()
        {
            if (initialized)
            {
                glDeleteFramebuffers(1, &fbo);
                initialized = false;
                fbo = 0;
            }
        }
        Framebuffer() {  }
        ~Framebuffer() { destroyFbo(); }

        Framebuffer(const Framebuffer&) = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;

        Framebuffer(Framebuffer&& other) noexcept
            : fbo(other.fbo), initialized(other.initialized)
        {
            other.fbo = 0;
            other.initialized = false;
        }

        Framebuffer& operator=(Framebuffer&& other) noexcept
        {
            if (this != &other)
            {
                destroyFbo();
                fbo = other.fbo;
                initialized = other.initialized;
                other.fbo = 0;
                other.initialized = false;
            }
            return *this;
        }

        void bind() { glBindFramebuffer(GL_FRAMEBUFFER, fbo); }
        void bindRead() { glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo); }
        void bindDraw() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo); }
        void clearDrawBuffer() { glDrawBuffer(GL_NONE); }
        void clearReadBuffer() { glReadBuffer(GL_NONE); }
        void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

        bool complete() { return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE; }
    };
}