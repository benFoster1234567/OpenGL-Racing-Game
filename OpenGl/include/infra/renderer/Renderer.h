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

namespace Engine::Infra 
{
	constexpr uint32_t MAX_LIGHTS = 100;

	struct StaticPointLightResource
	{
		glm::vec3 position{ 0.0f };
		glm::vec3 color{ 0.0f };
		float radius{ 0.0f };
		float intensity{ 1.0f };
	};

	struct StaticPointLight
	{
		glm::vec4 posRad = { 0.0f,0.0f,0.0f, 10.0f };
		glm::vec4 color = { 1.0f,1.0f,1.0f, 1.0f };
	};

	struct UboStaticPointLightData
	{
		StaticPointLight lights[100];
		int activeLightCount;
		int padding[3];
	};

	struct RenderCommand
	{
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 modelTransform;
		Core::ShaderData* shader;
		Core::MeshData* mesh;
		Core::MaterialData* material;
	};

	enum PolygonMode
	{
		FILL, LINE
	};

	

	class Renderer
	{
	private:
		std::vector<RenderCommand> renderQueue;
		std::map<Core::MeshData*, std::unique_ptr<GpuMesh>> gpuMeshCache{};
		std::map<Core::TextureData*, std::unique_ptr<GpuTexture>> gpuTextureCache{};
		//std::map<Core::TextureIdx, unsigned int> textureIdxToId{}; // Maps texture index to OpenGL texture ID. We can change this to a span set if we want to avoid the map overhead.
		std::vector<StaticPointLightResource> staticPointLights{};
		void cacheShader(Core::ShaderData* shaderData);
		void cacheTexture(Core::TextureData* textureData);
		void cacheMesh(Core::MeshData* meshData);
		void drawLights(Core::ShaderData* lightShader, size_t lightCount);
		
		unsigned int renderMode = 0;

		int polygonMode = LINE;

		Quad screenQuad{};

		glm::mat4 getLightProjectionMatrix() const
		{
			static glm::mat4 p = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
			return p;
		}

		size_t activeLightCount = 0;
		GLuint emptyVao{ 0 }; // for debug positions and such
		GLuint ubo{ 0 };
		GpuShader* DebugLightShader = nullptr;

		GpuShader* shadowShader = nullptr;
		GpuShader* depthShader = nullptr;

		ShadowMap shadowMap;
		ShadowCubeMap shadowCubemap;
	public:
		std::map<Core::ShaderData*, std::unique_ptr<GpuShader>> gpuShaderCache{};
	
	private:

		glm::mat4 getLightSpaceMatrix(const Engine::Infra::RenderCommand& command, const StaticPointLightResource& light)
		{
			auto lightProj = getLightProjectionMatrix();
			glm::mat4 modelMatrix = command.modelTransform;
			glm::vec3 position = glm::vec3(modelMatrix[3]);
			glm::mat4 viewMat = glm::lookAt(light.position, -light.position, glm::vec3(0, 1, 0));
			glm::mat4 lightSpaceMatrix = lightProj * viewMat;
			return lightSpaceMatrix;
		}

		void configureShadowShadersAndMatrices(const Engine::Infra::RenderCommand& command, const StaticPointLightResource& light)
		{
			auto lightProj = getLightProjectionMatrix();
			glm::mat4 modelMatrix = command.modelTransform;
			glm::vec3 position = glm::vec3(modelMatrix[3]);
			glm::mat4 viewMat = glm::lookAt(light.position, -light.position, glm::vec3(0, 1, 0));
			glm::mat4 lightSpaceMatrix = getLightSpaceMatrix(command, light);
			if (shadowShader != nullptr) 
			{
				shadowShader->use();

				glUniformMatrix4fv(glGetUniformLocation(shadowShader->getId(), "lightSpaceMatrix"), 1, GL_FALSE, (GLfloat*)&lightSpaceMatrix[0]);
				glUniformMatrix4fv(glGetUniformLocation(shadowShader->getId(), "model"), 1, GL_FALSE, (GLfloat*)&modelMatrix[0]);
			}
		}

	public:
		
		Renderer() : shadowMap{1024, 1024}, shadowCubemap{1024, 1024}
		{
			ilInit();
			iluInit();
			ilutRenderer(ILUT_OPENGL);
		}

		~Renderer() = default;

		void createShadowMap()
		{
			shadowMap.create();
			shadowMap.attachFramebufferToDepthBuffer();
		}

		void createShadowCubemap()
		{
			shadowCubemap.create();
			shadowCubemap.attachFramebufferToDepthBuffer();
		}

		void loadMeshes(std::vector<Core::MeshData*>& meshes);
		void loadShaders(std::vector<Core::ShaderData*>& shaders);
		void loadTextures(std::vector<Core::TextureData*>& textures);
		void loadLights(std::vector<StaticPointLightResource>& staticLights);
		void renderLights();
		void submit(RenderCommand command);

		void flush(size_t w = 800, size_t h = 600);

		void setPolygonMode(int m) { polygonMode = m; }

		void clear() const
		{
			glClearColor(1, 1, 1, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void flush();
	};
}