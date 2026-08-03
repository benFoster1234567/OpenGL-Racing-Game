#pragma once
#include <GL/glew.h>
#include "infra/renderer/assets/GPUMesh.h"
#include "infra/renderer/assets/GPUTexture.h"
#include "infra/renderer/assets/GpuShader.h"
#include "core/assets/MaterialData.h"

#include <map>
#include <string>
#include <vector>
#include "core/assets/AssetManager.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

namespace Engine::Infra 
{
	constexpr uint32_t MAX_LIGHTS = 100;

	struct StaticPointLightResource
	{
		glm::vec3 position{ 0.0f };
		glm::vec3 color{ 0.0f };
		float radius{ 0.0f };
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
		std::map<std::string, unsigned int> textureNameToId{};
		void cacheShader(Core::ShaderData* shaderData);
		void cacheMesh(Core::MeshData* meshData);
		int polygonMode = LINE;

		unsigned int loadTexture(const char* filename);

	public:
		Renderer() = default;
		~Renderer() = default;

		void loadTextureFromFile(const std::string& filePath, const std::string& name);

		void loadMeshes(std::vector<Core::MeshData*>& meshes);
		void loadShaders(std::vector<Core::ShaderData*>& shaders);
		void loadLights(std::vector<StaticPointLightResource>& staticLights);
		void submit(RenderCommand command);

		void setPolygonMode(int m) { polygonMode = m; }

		void clear() const
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		std::map<Core::ShaderData*, std::unique_ptr<GpuShader>> gpuShaderCache{};
		void flush();
	};
}