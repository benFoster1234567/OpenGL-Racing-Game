#pragma once
#include "Framebuffer.h"
#include "DepthMap.h"

namespace Engine::Infra 
{
	class ShadowMap
	{
	private:
		Framebuffer framebuffer;
		DepthMap depthMap;
		size_t shadowWidthPx{ 0 };
		size_t shadowHeightPx{ 0 };
		bool initialized = false;
		void setViewport()
		{
			glViewport(0, 0, shadowWidthPx, shadowHeightPx);
		}

	public:
		ShadowMap(size_t _width, size_t _height) 
			: shadowWidthPx{ _width }
			, shadowHeightPx{ _height }
			, framebuffer{}
			, depthMap { _width, _height } 
		{
		}

		size_t getWidth() const { return shadowWidthPx; }
		size_t getHeight() const { return shadowHeightPx; }

		void attachFramebufferToDepthBuffer()
		{
			framebuffer.bind();
			depthMap.attach();
			framebuffer.clearDrawBuffer();
			framebuffer.clearReadBuffer();
			framebuffer.unbind();
		}

		void create()
		{
			if (initialized) return;
			initialized = true;
			framebuffer.generateFbo();
			depthMap.create();
		}

		void bindDepthMap()
		{
			depthMap.bind();
		}

		//call prior to rendering the scene
		void bindFramebuffer()
		{
			setViewport();
			framebuffer.bind();
		}

		void unbindFrameBuffer() { framebuffer.unbind(); }

	};
}