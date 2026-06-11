#include "infra/renderer/assets/GPUMesh.h"

Engine::Infra::GpuMesh::GpuMesh(Engine::Core::MeshData* meshData)
	: meshData(meshData)
{
}

void Engine::Infra::GpuMesh::genBuffers()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	if (!meshData->attributes.empty()) {
		const auto& firstAttr = meshData->attributes[0];
		vertexCount = static_cast<GLsizei>(firstAttr.data.size() / firstAttr.size);
		std::cout << "vertexCount: " << vertexCount << "\n";
	}

	for (const auto& va : meshData->attributes)
	{
		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, va.data.size() * sizeof(float), va.data.data() , GL_STATIC_DRAW);
		glVertexAttribPointer(va.index, va.size, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(va.index);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Engine::Infra::GpuMesh::draw() const
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindVertexArray(0);
}
