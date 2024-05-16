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

void Mesh::Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue)
{
	const VkDeviceSize vertexBufferSize{ GetVerticesSizeInByte() };
	m_VertexBuffer = VertexBuffer(physicalDevice, device, commandPool, graphicQueue, vertexBufferSize, m_Vertices.data());

	const VkDeviceSize indexBufferSize{ GetIndicesSizeInByte() };
	m_IndexBuffer = IndexBuffer(physicalDevice, device, commandPool, graphicQueue, indexBufferSize, m_Indices.data());
}

void Mesh::Draw(VkCommandBuffer commandBuffer) const
{
	m_VertexBuffer.BindVertexBuffer(commandBuffer);
	m_IndexBuffer.BindIndexBuffer(commandBuffer, VK_INDEX_TYPE_UINT32);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);
}

void Mesh::Cleanup(VkDevice device) const
{
	m_VertexBuffer.Cleanup(device);
	m_IndexBuffer.Cleanup(device);
}
