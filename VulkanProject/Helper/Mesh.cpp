#include "Mesh.h"

void Mesh::AddIndex(uint32_t index)
{
	m_Indices.emplace_back(index);
}

size_t Mesh::GetIndicesSizeInByte() const
{
	return m_Indices.size() * sizeof(uint32_t);
}

size_t Mesh::GetIndicesSize() const
{
	return m_Indices.size();
}