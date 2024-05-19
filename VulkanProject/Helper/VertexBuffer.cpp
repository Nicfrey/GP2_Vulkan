#include "VertexBuffer.h"

#include <stdexcept>

#include "Helper.h"
#include "Vertex.h"

VkBuffer& DataBuffer::GetBuffer()
{
	return m_Buffer;
}

VkDeviceMemory& DataBuffer::GetBufferMemory()
{
	return m_DeviceMemory;
}

void DataBuffer::Cleanup(VkDevice device) const
{
	vkDestroyBuffer(device, m_Buffer, nullptr);
	vkFreeMemory(device, m_DeviceMemory, nullptr);
}

void DataBuffer::CreateBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size,VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferCreateInfo{};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = size; // size of the buffer
	bufferCreateInfo.usage = usage;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult result{ vkCreateBuffer(device,&bufferCreateInfo,nullptr,&buffer) };
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Vertex buffer not correclty initialized");
	}

	// buffer created but it has no memory assigned to it
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

	result = vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to locate vertex buffer memory");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

uint32_t DataBuffer::FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i{}; i < memProperties.memoryTypeCount; ++i)
	{
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type");
}

void DataBuffer::CopyBuffer(VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	const VkCommandBuffer commandBuffer{ BeginSingleTimeCommands(device,commandPool) };

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	EndSingleTimeCommands(commandBuffer, device, graphicQueue, commandPool);
}

VkCommandBuffer DataBuffer::BeginSingleTimeCommands(VkDevice device, VkCommandPool commandPool)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void DataBuffer::EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkDevice device, VkQueue queue,
	VkCommandPool commandPool)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

VertexBuffer::VertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device,VkCommandPool commandPool,VkQueue graphicQueue , VkDeviceSize size, const void* bufferData): DataBuffer()
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(device,physicalDevice,size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,stagingBuffer, stagingBufferMemory);
	
	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, size, 0, &data);
	memcpy(data, bufferData, size); // TODO Replace with other data parameter
	vkUnmapMemory(device, stagingBufferMemory);

	CreateBuffer(device, physicalDevice, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer, m_DeviceMemory);
	CopyBuffer(device, commandPool, graphicQueue,stagingBuffer, m_Buffer, size);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void VertexBuffer::BindVertexBuffer(VkCommandBuffer commandBuffer) const
{
	VkBuffer vertexBuffers[] = { m_Buffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}

IndexBuffer::IndexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,
                         VkQueue graphicQueue, VkDeviceSize size, const void* bufferData)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(device, physicalDevice,size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, size, 0, &data);
	memcpy(data, bufferData, size); 
	vkUnmapMemory(device, stagingBufferMemory);

	CreateBuffer(device,physicalDevice,size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer, m_DeviceMemory);
	CopyBuffer(device, commandPool,graphicQueue,stagingBuffer, m_Buffer, size);
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void IndexBuffer::BindIndexBuffer(VkCommandBuffer commandBuffer, VkIndexType indexType) const
{
	vkCmdBindIndexBuffer(commandBuffer, m_Buffer, 0, indexType);
}
