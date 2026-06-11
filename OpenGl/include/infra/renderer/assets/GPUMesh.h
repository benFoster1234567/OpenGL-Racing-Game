#pragma once
#include <GL/glew.h>
#include "core/assets/MeshData.h"

namespace Engine::Infra
{
	class GpuMesh
	{
	private:
		GLuint VAO = 0;
		GLsizei vertexCount = 0;
		Engine::Core::MeshData* meshData;
	public:

		GpuMesh(Engine::Core::MeshData* meshData);
		GpuMesh() : meshData(nullptr) {}
		GpuMesh(GpuMesh&&) = default;
		
		GpuMesh(const GpuMesh&) = delete; //non copyable because of opengl buffer ids

		~GpuMesh() = default;
		void genBuffers();
		void draw() const;
	};
}