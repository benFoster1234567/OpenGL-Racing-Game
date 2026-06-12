#pragma once
#include <GL/glew.h>
#include "core/ecs/Entity.h"
#include "infra/renderer/assets/GPUMesh.h"
#include "infra/renderer/assets/GPUTexture.h"
#include "infra/renderer/assets/GpuShader.h"
#include "core/assets/MaterialData.h"

#include <map>
#include <string>
#include <vector>
#include "core/assets/AssetManager.h"

#include <glm/ext/matrix_float4x4.hpp>

namespace Engine::Infra 
{
	struct RenderCommand
	{
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 modelTransform;
		Core::ShaderData* shader;
		Core::MeshData* mesh;
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

		void cacheShader(Core::ShaderData* shaderData);
		void cacheMesh(Core::MeshData* meshData);

		int polygonMode = LINE;

	public:
		Renderer() = default;
		~Renderer() = default;

		void loadMeshes(std::vector<Core::MeshData*>& meshes);
		void loadShaders(std::vector<Core::ShaderData*>& shaders);
		void submit(RenderCommand command);

		void setPolygonMode(int m) { polygonMode = m; }

		std::map<Core::ShaderData*, std::unique_ptr<GpuShader>> gpuShaderCache{}; 

		void flush();
	};

}