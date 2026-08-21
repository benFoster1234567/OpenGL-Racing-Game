#pragma once

#include "DepthCubeMap.h"
#include "Framebuffer.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine::Infra
{
	class ShadowCubeMap
	{
	private:
		size_t width{};
		size_t height{};

		Framebuffer framebuffer;
		DepthCubeMap depthCubemap;
		
		//glm::mat4 lightSpaceTransform{};
		std::vector<glm::mat4> shadowTransforms{};

		glm::vec3 lightPosition{};

		void genShadowTransforms()
		{
			shadowTransforms.clear();
			float aspect = float(width) / float(height);
			float nnear = 0.1f;
			float ffar = 25.0f;
			glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, nnear, ffar);

			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
		}

	public:
		ShadowCubeMap(size_t _width, size_t _height)
			: framebuffer{}, depthCubemap{_width, _height}
			, width{_width}
			, height{_height}
		{
			genShadowTransforms();
		}

		void setLightPosition(glm::vec3 position)
		{
			lightPosition = position;
			genShadowTransforms();
		}

		void create()
		{
			framebuffer.generateFbo();
			depthCubemap.generate();
			depthCubemap.bind();
			depthCubemap.prepareCubemapFaces();
			//genShadowTransforms();
		}
		
		void destroy()
		{
			framebuffer.destroyFbo();
		}

		void attachFramebufferToDepthBuffer()
		{
			framebuffer.bind();
			depthCubemap.attach();
			framebuffer.clearDrawBuffer();
			framebuffer.clearReadBuffer();
			framebuffer.unbind();
		}

		void setViewport()		{ depthCubemap.setViewport(); }
		void bindFramebuffer()	{ framebuffer.bind(); }
		void unbindFramebuffer() { framebuffer.unbind(); }
		void bindDepthCubemap() { depthCubemap.bind(); }
		void sendTransformsToShader(GLuint shader)
		{
			GLuint loc = glGetUniformLocation(shader, "shadowMatrices");
			glUniformMatrix4fv(loc,6,  GL_FALSE, glm::value_ptr(shadowTransforms[0]));

			GLuint locLight = glGetUniformLocation(shader, "lightPos");
			glUniform3fv(locLight, 1, glm::value_ptr(lightPosition));

			GLuint locFar = glGetUniformLocation(shader, "far_plane");
			glUniform1f(locFar, 25.0f); // Matches 'ffar' from genShadowTransforms
		}

	};


}