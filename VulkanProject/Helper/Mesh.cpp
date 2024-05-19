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

void Mesh::Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue,
	VkDescriptorSetLayout descriptorSetLayout)
{
	const VkDeviceSize indexBufferSize{ GetIndicesSizeInByte() };
	m_IndexBuffer = IndexBuffer(physicalDevice, device, commandPool, graphicQueue, indexBufferSize, m_Indices.data());
}

void Mesh::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const
{
	m_IndexBuffer.BindIndexBuffer(commandBuffer, VK_INDEX_TYPE_UINT32);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);
}

void Mesh::Cleanup(VkDevice device) const
{
	m_IndexBuffer.Cleanup(device);
}
