#pragma once
#include <GL/glew.h>
#include "infra/renderer/assets/GPUMesh.h"
#include "infra/renderer/assets/GPUTexture.h"
#include "infra/renderer/assets/GpuShader.h"
#include "core/assets/MaterialData.h"
#include "infra/engine/importFuncs.h"
#include <map>
#include <string>
#include <vector>
#include "core/assets/AssetManager.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include "Quad.h"
#include "core/assets/AssetIds.h"
#include "SparseSet.h"
#include "PointlightLoader.h"

namespace Engine::Infra
{
	struct RenderCommand
	{
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 modelTransform;
		glm::vec2 uvScale;
		Core::ShaderId shader;
		Core::MeshId mesh;
		Core::MaterialData* material;
	};

	enum PolygonMode
	{
		FILL, LINE
	};

	class Renderer
	{

	public:
		PointlightLoader pointlightLoader{};

	private:

		friend class GpuAssetLoader;
		std::vector<RenderCommand> m_renderQueue;

		SparseSet<std::unique_ptr<GpuTexture>, Core::AssetIdMax, Core::AssetCapacity> m_gpuTextureCache{};
		SparseSet<std::unique_ptr<GpuMesh>, Core::AssetIdMax, Core::AssetCapacity> m_gpuMeshCache{};
		SparseSet<std::unique_ptr<GpuShader>, Core::AssetIdMax, Core::AssetCapacity> m_gpuShaderCache{};

		std::vector<StaticPointLightResource> m_staticPointLights{};

		void cacheShader(Core::ShaderId shaderId, Core::ShaderData* shaderData);
		void cacheTexture(Core::TextureId textureId, Core::TextureData* textureData);
		void cacheMesh(Core::MeshId meshId, Core::MeshData* meshData);

		void drawLights(Core::ShaderId shaderId, size_t lightCount);

		unsigned int m_renderMode = 0;


		int m_polygonMode = LINE;

		Quad m_screenQuad{};

		glm::mat4 getLightProjectionMatrix() const
		{
			static glm::mat4 p = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
			return p;
		}

		struct
		{
			size_t numLights{ 0 }; // <= 16
			std::vector<glm::vec3> lightPositions{};
			std::vector<glm::mat4> shadowTransforms{};
		}m_staticPointlightData;

		size_t m_activeLightCount = 0;
		GLuint m_emptyVao{ 0 }; // for debug positions and such
		GLuint m_ubo{ 0 };
		GpuShader* m_debugLightShader = nullptr;

		GpuShader* m_shadowCubemapShader = nullptr;
		GLuint m_depthCubemapId = 0;
		GLuint m_depthMapFbo = 0;

		glm::vec3 m_lightPos{};

		float m_near{};
		float m_far{};
		std::vector<glm::mat4> m_shadowTransforms;

		glm::mat4 getLightSpaceMatrix(const Engine::Infra::RenderCommand& command, const StaticPointLightResource& light)
		{
			auto lightProj = getLightProjectionMatrix();
			glm::mat4 modelMatrix = command.modelTransform;
			glm::vec3 position = glm::vec3(modelMatrix[3]);
			glm::mat4 viewMat = glm::lookAt(light.position, -light.position, glm::vec3(0, 1, 0));
			glm::mat4 lightSpaceMatrix = lightProj * viewMat;
			return lightSpaceMatrix;
		}

		std::vector<glm::mat4> getTransforms(glm::vec3 lp)
		{
			std::vector<glm::mat4> transforms{};

			const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
			float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;

			glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, m_near, m_far);

			transforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			transforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			transforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
			transforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
			transforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
			transforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
			return transforms;
		}

	public:

		Renderer()
		{
			ilInit();
			iluInit();
			ilutRenderer(ILUT_OPENGL);

			m_near = 1.0f;
			m_far = 25.0f;
		}

		void prepareDepthCubemapArray();

		void prepareDepthCubemap();

		void renderToShadowCubemapArray(size_t w, size_t h);

		~Renderer() = default;

		void setShadowCubemapShader(Core::ShaderId shader)
		{
			m_shadowCubemapShader = m_gpuShaderCache.get(shader).get();
		}

		void loadLights(std::vector<StaticPointLightResource> staticLights);
		void loadShadowingLights(glm::vec3 cameraOrigin);
		void updatePointlightSources(glm::vec3 cameraOrigin)
		{
			pointlightLoader.updatePointShadowSources(cameraOrigin, m_near, m_far);
		}

		void submit(RenderCommand command);

		void flush(size_t w = 800, size_t h = 600);

		void setPolygonMode(int m) { m_polygonMode = m; }

		void clear() const
		{
			glClearColor(1, 1, 1, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

	};
}