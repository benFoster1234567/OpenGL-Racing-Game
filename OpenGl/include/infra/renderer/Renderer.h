#pragma once
#include "core/ecs/Entity.h"
#include "infra/renderer/assets/GPUMesh.h"
#include "infra/renderer/assets/GPUTexture.h"
#include "core/assets/MaterialData.h"

#include <map>
#include <string>
#include <vector>
#include "core/assets/MeshData.h"
#include "core/assets/TextureData.h"
#include "core/assets/MaterialData.h"

#include <GL/glew.h>

namespace Engine::Infra 
{

	struct RenderCommand
	{
		GpuTexture* diffuse;
		GpuTexture* normal;
		GpuMesh* mesh;
		Core::MaterialData* material;
		Core::Transform* transform;
	};

	class Renderer
	{
	private:
		std::map<Core::MeshData*, GpuMesh> gpuMeshes{};
		std::map<Core::TextureData*, GpuTexture> gpuTextures{};


		std::vector<RenderCommand> renderQueue{};

		void sendMaterialToShader(Core::MaterialInfo m, GLuint shaderId);
		void sendTransformToShader(Core::Transform* t, GLuint shaderId);
	public:
		void clearColor();
		void submit(Core::Entity entity);
		void flush(); //renders all in render queue and then clears it.

	};
}