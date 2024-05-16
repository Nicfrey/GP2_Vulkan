#pragma once
#include "vulkan/vulkan.h"

class DataBuffer
{
public:
	DataBuffer() = default;
	virtual ~DataBuffer() = default;

	VkBuffer& GetBuffer();
	VkDeviceMemory& GetBufferMemory();
	void Cleanup(VkDevice device) const;

protected:
	VkBuffer m_Buffer;
	VkDeviceMemory m_DeviceMemory;

	static void CreateBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	static uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	static void CopyBuffer(VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};

class VertexBuffer final : public DataBuffer
{
public:
	VertexBuffer() = default;
	VertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, VkDeviceSize size, const void* bufferData);
	~VertexBuffer() override = default;

	void BindVertexBuffer(VkCommandBuffer commandBuffer) const;
};

class IndexBuffer final : public DataBuffer
{
public:
	IndexBuffer() = default;
	IndexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, VkDeviceSize size, const void* bufferData);
	~IndexBuffer() override = default;

	void BindIndexBuffer(VkCommandBuffer commandBuffer, VkIndexType indexType) const;
};