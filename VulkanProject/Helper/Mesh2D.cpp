#include "Helper/Mesh2D.h"

void Mesh2D::AddVertex(const Vertex2D& vertex)
{
	m_Vertices.push_back(vertex);
}

void Mesh2D::AddVertex(const glm::vec2& position, const glm::vec3& color)
{
	m_Vertices.push_back({ position, color });
}

void Mesh2D::AddVertex(const glm::vec2& position)
{
	m_Vertices.push_back({ position, glm::vec3(1.0f) });
}

size_t Mesh2D::GetVerticesSizeInByte() const
{
	return m_Vertices.size() * sizeof(Vertex2D);
}

size_t Mesh2D::GetVerticesSize() const
{
	return m_Vertices.size();
}

void Mesh2D::Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue,VkDescriptorSetLayout descriptorSetLayout)
{
	const VkDeviceSize vertexBufferSize{ GetVerticesSizeInByte() };
	m_VertexBuffer = VertexBuffer(physicalDevice, device, commandPool, graphicQueue, vertexBufferSize, m_Vertices.data());

	const VkDeviceSize indexBufferSize{ GetIndicesSizeInByte() };
	m_IndexBuffer = IndexBuffer(physicalDevice, device, commandPool, graphicQueue, indexBufferSize, m_Indices.data());
}

void Mesh2D::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipeline) const
{
	m_VertexBuffer.BindVertexBuffer(commandBuffer);
	m_IndexBuffer.BindIndexBuffer(commandBuffer, VK_INDEX_TYPE_UINT32);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);
}

void Mesh2D::Cleanup(VkDevice device) const
{
	m_VertexBuffer.Cleanup(device);
	m_IndexBuffer.Cleanup(device);
}
