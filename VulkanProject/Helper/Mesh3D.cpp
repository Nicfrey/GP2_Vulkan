#include "Helper/Mesh3D.h"

#include <stdexcept>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Mesh3D::Mesh3D(int maxFrameInFlight, const std::string& texture): m_MaxFrameInFlight{ maxFrameInFlight }, m_TextureImage{ texture }
{
	AddVertex( {-0.5f,-0.5f}, {1.f,0.f,0.f} , {1.0f,0.f} );
	AddVertex( {0.5f,-0.5f},{0.f,1.f,0.f},{0.f,0.f} );
	AddVertex( {0.5f,0.5f},{0.f,0.f,1.f},{0.f,1.f} );
	AddVertex( {-0.5f,0.5f},{1.f,1.f,1.f}, {1.f,1.f} );
	AddIndex(0);
	AddIndex(1);
	AddIndex(2);
	AddIndex(2);
	AddIndex(3);
	AddIndex(0);
}

void Mesh3D::Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, VkDescriptorSetLayout descriptorLayout)
{
	m_TextureImage.Init(device, physicalDevice, commandPool, graphicQueue);
	const VkDeviceSize vertexBufferSize{ GetVerticesSizeInByte() };
	m_VertexBuffer = VertexBuffer(physicalDevice, device, commandPool, graphicQueue, vertexBufferSize, m_Vertices.data());

	const VkDeviceSize indexBufferSize{ GetIndicesSizeInByte() };
	m_IndexBuffer = IndexBuffer(physicalDevice, device, commandPool, graphicQueue, indexBufferSize, m_Indices.data());

	CreateUniformBuffers(physicalDevice, device);

	// CreateDescriptorPool
	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(m_MaxFrameInFlight);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(m_MaxFrameInFlight);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(m_MaxFrameInFlight);

	VkResult result{ vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_DescriptorPool) };
	if(result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create descriptor pool");
	}

	// CreateDescriptorSets
	const std::vector layouts(m_MaxFrameInFlight, descriptorLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_DescriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(m_MaxFrameInFlight);
	allocInfo.pSetLayouts = layouts.data();

	m_DescriptorSets.resize(m_MaxFrameInFlight);
	result = vkAllocateDescriptorSets(device, &allocInfo, m_DescriptorSets.data());
	if(result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate descriptor sets");
	}

	for(size_t i{}; i < m_MaxFrameInFlight; ++i)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = m_UniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = m_TextureImage.GetTextureImageView();
		imageInfo.sampler = m_TextureImage.GetTextureSampler();
		

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = m_DescriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = m_DescriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

}

void Mesh3D::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const
{
	m_VertexBuffer.BindVertexBuffer(commandBuffer);
	m_IndexBuffer.BindIndexBuffer(commandBuffer, VK_INDEX_TYPE_UINT32);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_DescriptorSets[currentFrame], 0, nullptr);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);
}

void Mesh3D::Update(uint32_t currentImage, float deltaTime)
{
	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), deltaTime * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;
	std::memcpy(m_UniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void Mesh3D::Cleanup(VkDevice device) const
{
	m_TextureImage.Cleanup(device);
	m_VertexBuffer.Cleanup(device);
	m_IndexBuffer.Cleanup(device);
	vkDestroyDescriptorPool(device, m_DescriptorPool, nullptr);
	for(size_t i{}; i < m_UniformBuffers.size(); ++i)
	{
		vkDestroyBuffer(device, m_UniformBuffers[i], nullptr);
		vkFreeMemory(device, m_UniformBuffersMemory[i], nullptr);
	}
}

void Mesh3D::AddVertex(const Vertex3D& vertex)
{
	m_Vertices.push_back(vertex);
}

void Mesh3D::AddVertex(const glm::vec2& position, const glm::vec3& color)
{
	m_Vertices.push_back({ position, color });
}

void Mesh3D::AddVertex(const glm::vec2& position)
{
	m_Vertices.push_back({ position, {1.f,1.f,1.f} });
}

void Mesh3D::AddVertex(const glm::vec2& position, const glm::vec3& color, const glm::vec2& textCoord)
{
	m_Vertices.push_back({ position, color, textCoord });
}

size_t Mesh3D::GetVerticesSizeInByte() const
{
	return m_Vertices.size() * sizeof(Vertex3D);
}

size_t Mesh3D::GetVerticesSize() const
{
	return m_Vertices.size();
}

void Mesh3D::CreateUniformBuffers(VkPhysicalDevice physicalDevice, VkDevice device)
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	m_UniformBuffers.resize(m_MaxFrameInFlight);
	m_UniformBuffersMemory.resize(m_MaxFrameInFlight);
	m_UniformBuffersMapped.resize(m_MaxFrameInFlight);
	for (size_t i{}; i < m_MaxFrameInFlight; ++i)
	{
		DataBuffer::CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffers[i], m_UniformBuffersMemory[i]);
		vkMapMemory(device, m_UniformBuffersMemory[i], 0, bufferSize, 0, &m_UniformBuffersMapped[i]);
	}
}
