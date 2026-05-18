#pragma once
#include "Entity.h"
#include "GPUMesh.h"
#include "GPUTexture.h"
#include "Material.h"
#include "ShaderProgram.h"
#include <map>
#include <string>
#include <vector>
#include "MeshData.h"
#include "Texture.h"
#include <GL/glew.h>

struct RenderCommand
{
	ShaderProgram* shader;
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
	std::map<std::string, ShaderProgram> shaders;

	std::vector<RenderCommand> renderQueue{};

	void sendMaterialToShader(MaterialInfo m, GLuint shaderId);
	void sendTransformToShader(Transform* t, GLuint shaderId);
public:
	
	void submit(Entity entity);
	void flush();

};