#include "Mesh.h"

void Mesh::AddVertex(const Vertex2D& vertex)
{
	m_Vertices.emplace_back(vertex);
}

void Mesh::AddVertex(const glm::vec2& position, const glm::vec3& color)
{
	AddVertex(Vertex2D{ position, color });
}

void Mesh::AddVertex(const glm::vec2& position)
{
	// White color
	AddVertex(Vertex2D{ position, glm::vec3(1.f) });
}

void Mesh::AddIndex(uint32_t index)
{
	m_Indices.emplace_back(index);
}

size_t Mesh::GetVerticesSizeInByte() const
{
	return m_Vertices.size() * sizeof(Vertex2D);
}

size_t Mesh::GetVerticesSize() const
{
	return m_Vertices.size();
}

size_t Mesh::GetIndicesSizeInByte() const
{
	return m_Indices.size() * sizeof(uint32_t);
}

size_t Mesh::GetIndicesSize() const
{
	return m_Indices.size();
}