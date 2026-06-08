#pragma once
#include "core/ecs/Entity.h"
#include "infra/renderer/assets/GPUMesh.h"
#include "infra/renderer/assets/GPUTexture.h"
#include "infra/renderer/assets/GpuShader.h"
#include "core/assets/MaterialData.h"

#include <map>
#include <string>
#include <vector>
#include "core/assets/MeshData.h"
#include "core/assets/TextureData.h"
#include "core/assets/ShaderData.h"
#include "core/assets/MaterialData.h"

#include <GL/glew.h>

namespace Engine::Infra 
{

	struct RenderCommand
	{
		GpuMesh* mesh;
		GpuTexture* diffuse;
		GpuTexture* normal;
		GpuShader* shader;
		Core::MaterialData* material;
		Core::Transform* transform;
	};

	class Renderer
	{
	private:
		std::map < Core::MeshData*, std::unique_ptr<GpuMesh>> gpuMeshes{};
		std::map<Core::TextureData*, std::unique_ptr<GpuTexture>> gpuTextures{};
		std::map<Core::ShaderData*, std::unique_ptr<GpuShader>> gpuShaders{};


		std::vector<RenderCommand> renderQueue{};


		void executeDraw(RenderCommand cmd);
	public:
		void clearColor();
		void submit(Core::Entity entity);
		void flush(); //renders all in render queue and then clears it.

	};
}