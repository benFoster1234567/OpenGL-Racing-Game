#pragma once
#include "core/ecs/Entity.h"
#include "infra/GPUMesh.h"
#include "infra/GPUTexture.h"
#include "core/assets/MaterialData.h"

#include <map>
#include <string>
#include <vector>
#include "core/assets/MeshData.h"
#include "core/assets/TextureData.h"

#include <GL/glew.h>

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
	std::map<TextureData*, GpuTexture> gpuTextures{};
	

	std::vector<RenderCommand> renderQueue{};

	void sendMaterialToShader(MaterialInfo m, GLuint shaderId);
	void sendTransformToShader(Transform* t, GLuint shaderId);
public:
	
	void submit(Entity entity);
	void flush();

};