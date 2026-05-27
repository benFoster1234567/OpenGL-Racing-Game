#pragma once
#include "core/assets/MeshData.h"

namespace Engine::Infra
{
	class GpuMesh
	{
	private:
		GLuint VBO = 0, VAO = 0, EBO = 0;
		Engine::Core::MeshData* meshData;
	public:
		GpuMesh(Engine::Core::MeshData* meshData);
		~GpuMesh() = default;
		void genBuffers();
		void draw() const;
	};
}