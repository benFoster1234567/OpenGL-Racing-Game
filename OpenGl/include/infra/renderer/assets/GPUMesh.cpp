#include "infra/renderer/assets/GPUMesh.h"

Engine::Infra::GpuMesh::GpuMesh(Engine::Core::MeshData* meshData)
	: m_meshData(meshData)
{
}

void Engine::Infra::GpuMesh::genBuffers()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	if (!m_meshData->m_attributes.empty()) {
		const auto& firstAttr = m_meshData->m_attributes[0];
		m_vertexCount = static_cast<GLsizei>(firstAttr.data.size() / firstAttr.size);
		std::cout << "vertexCount: " << m_vertexCount << "\n";
	}

	for (const auto& va : m_meshData->m_attributes)
	{
		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, va.data.size() * sizeof(float), va.data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(va.index, va.size, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(va.index);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Engine::Infra::GpuMesh::draw() const
{
	auto renderAs = m_meshData->m_meshType == Engine::Core::MeshType::Fill ? GL_TRIANGLES : GL_LINES;
	glBindVertexArray(m_vao);
	glDrawArrays(renderAs, 0, m_vertexCount);
	glBindVertexArray(0);

}
