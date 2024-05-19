#include "Shader.h"

#include <stdexcept>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Descriptor.h"
#include "Helper.h"
#include "Vertex.h"
#include "VertexBuffer.h"


void Shader::Initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice)
{
	CreateUniformBuffers(physicalDevice, device);
    m_Descriptor = std::make_unique<Descriptor>(device);
}

void Shader::Update(uint32_t currentFrame, float deltaTime, VkExtent2D swapchainExtent)
{
	m_Test += deltaTime;
	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), m_Test * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(90.f), swapchainExtent.width / static_cast<float>(swapchainExtent.height), 0.1f, 100.0f);
	ubo.proj[1][1] *= -1;
	std::memcpy(m_UniformBuffersMapped[currentFrame], &ubo, sizeof(ubo));
}

void Shader::Cleanup(const VkDevice& device)
{
	m_Descriptor->Cleanup(device);
	for (size_t i{ 0 }; i < Helper::MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroyBuffer(device, m_UniformBuffer[i].GetBuffer(), nullptr);
		vkFreeMemory(device, m_UniformBuffer[i].GetBufferMemory(), nullptr);
	}
}

void Shader::BindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t currentFrame)
{
	m_Descriptor->BindDescriptorSets(commandBuffer, pipelineLayout,currentFrame);
}

void Shader::CreateUniformBuffers(VkPhysicalDevice physicalDevice, VkDevice device)
{
	m_UniformBuffer.resize(Helper::MAX_FRAMES_IN_FLIGHT);
	m_UniformBuffersMapped.resize(Helper::MAX_FRAMES_IN_FLIGHT);
	for (size_t i{}; i < Helper::MAX_FRAMES_IN_FLIGHT; ++i)
	{
		constexpr VkDeviceSize bufferSize = sizeof(UniformBufferObject);
		DataBuffer::CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffer[i].GetBuffer(), m_UniformBuffer[i].GetBufferMemory());
		vkMapMemory(device, m_UniformBuffer[i].GetBufferMemory(), 0, bufferSize, 0, &m_UniformBuffersMapped[i]);
	}
}

void Shader::CreateDescriptorSets(VkDevice device, VkDescriptorSetLayout descriptorLayout, const std::unique_ptr<TextureImage>& textureImage) 
{
	m_Descriptor->CreateDescriptorSets(device, descriptorLayout, m_UniformBuffer, textureImage);
}