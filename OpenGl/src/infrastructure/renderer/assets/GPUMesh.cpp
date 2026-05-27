#include "infra/renderer/assets/GPUMesh.h"

Engine::Infra::GpuMesh::GpuMesh(Engine::Core::MeshData* meshData)
	: meshData(meshData)
{
}

void Engine::Infra::GpuMesh::genBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBufferData(GL_ARRAY_BUFFER, meshData->vertices.size() * sizeof(Engine::Core::Vertex), meshData->vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (void*)offsetof(Core::Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (void*)offsetof(Core::Vertex, texCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Engine::Infra::GpuMesh::draw() const
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, meshData->vertices.size());
	glBindVertexArray(0);
}
