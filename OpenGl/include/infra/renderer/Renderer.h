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
#include "Shadows.h"
#include "Quad.h"
#include "ShadowCubeMap.h"
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
	private:

		friend class GpuAssetLoader;
		std::vector<RenderCommand> renderQueue;
		
		SparseSet<std::unique_ptr<GpuTexture>, Core::AssetIdMax, Core::AssetCapacity> gpuTextureCache{};
		SparseSet<std::unique_ptr<GpuMesh>, Core::AssetIdMax, Core::AssetCapacity> gpuMeshCache{};
		SparseSet<std::unique_ptr<GpuShader>, Core::AssetIdMax, Core::AssetCapacity> gpuShaderCache{};
		
		std::vector<StaticPointLightResource> staticPointLights{};
		
		void cacheShader(Core::ShaderId shaderId, Core::ShaderData* shaderData);
		void cacheTexture(Core::TextureId textureId, Core::TextureData* textureData);
		void cacheMesh(Core::MeshId meshId, Core::MeshData* meshData);
		
		void drawLights(Core::ShaderId shaderId, size_t lightCount);
		
		unsigned int renderMode = 0;

		PointlightLoader pointlightLoader{};

		int polygonMode = LINE;

		Quad screenQuad{};

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
		}staticPointlightData;

		size_t activeLightCount = 0;
		GLuint emptyVao{ 0 }; // for debug positions and such
		GLuint ubo{ 0 };
		GpuShader* DebugLightShader = nullptr;

		GpuShader* shadowCubemapShader = nullptr;
		GLuint depthCubemapId = 0;
		GLuint depthMapFBO = 0;

		glm::vec3 lightPos{};

		float nnear{};
		float ffar{};
		std::vector<glm::mat4> shadowTransforms;

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

			glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, nnear, ffar);

			transforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			transforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			transforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
			transforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
			transforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
			transforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
			return transforms;
		}

		std::vector<glm::mat4> getTransformCubemapArray(std::vector<glm::vec3> lightPositions);

	public:
		
		Renderer()
		{
			ilInit();
			iluInit();
			ilutRenderer(ILUT_OPENGL);

			nnear = 1.0f;
			ffar = 25.0f;
		}

		void prepareDepthCubemapArray();

		void prepareDepthCubemap();

		void renderToShadowCubemapArray(size_t w, size_t h);

		void renderToShadowCubemap(size_t w, size_t h);

		~Renderer() = default;

		void setShadowCubemapShader(Core::ShaderId shader)
		{
			shadowCubemapShader = gpuShaderCache.get(shader).get();
		}

		void loadLights(std::vector<StaticPointLightResource> staticLights);
		void loadShadowingLights(std::vector<StaticPointLightResource> staticLights);

		void renderLights();

		void submit(RenderCommand command);

		void flush(size_t w = 800, size_t h = 600);

		void setPolygonMode(int m) { polygonMode = m; }

		void clear() const
		{
			glClearColor(1, 1, 1, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		
	};
}