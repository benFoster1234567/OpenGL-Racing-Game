#pragma once
#include <GL/glew.h>
#include "core/assets/MeshData.h"

namespace Engine::Infra
{
	class GpuMesh
	{
	private:
		GLuint m_vao = 0;
		GLsizei m_vertexCount = 0;
		Engine::Core::MeshData* m_meshData;
	public:

		GpuMesh(Engine::Core::MeshData* meshData);
		GpuMesh() : m_meshData(nullptr) {}
		GpuMesh(GpuMesh&&) = default;

		GpuMesh(const GpuMesh&) = delete; //non copyable because of opengl buffer ids

		~GpuMesh() = default;
		void genBuffers();
		void draw() const;
	};
}