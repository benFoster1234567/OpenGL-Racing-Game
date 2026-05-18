#pragma once
#include "Entity.h"
#include "GPUMesh.h"
#include "GPUTexture.h"
#include "Material.h"
#include <map>

struct RenderCommand
{
	GpuMesh* mesh;
	GpuTexture* diffuse;
	GpuTexture* normal;
	MaterialInfo material;
	Transform* transform;
};

class Renderer
{
private:
	std::map<MeshData*, GpuMesh> gpuMeshes{};
	std::map<Texture*, GpuTexture> gpuTextures{};
	std::vector<RenderCommand> renderQueue{};
public:
	void submit(Entity entity);
	void flush();

};