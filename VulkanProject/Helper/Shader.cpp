#include "Shader.h"

#include <stdexcept>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Descriptor.h"
#include "Vertex.h"
#include "VertexBuffer.h"

VkDescriptorSetLayout& Shader::GetDescriptorSetLayout()
{
	return m_DescriptorSetLayout;
}

void Shader::Initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice,
                        const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const int& maxFrameInFlight)
{
	m_MaxFrameInFlight = maxFrameInFlight;
	CreateDescriptorSetLayout(device);
	CreateUniformBuffers(physicalDevice, device);
    m_Descriptor = std::make_unique<Descriptor>(device, maxFrameInFlight);
}

void Shader::Update(uint32_t currentFrame, float deltaTime)
{
	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), deltaTime * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;
	std::memcpy(m_UniformBuffersMapped[currentFrame], &ubo, sizeof(ubo));
}

void Shader::Cleanup(const VkDevice& device)
{
	vkDestroyDescriptorSetLayout(device, m_DescriptorSetLayout, nullptr);
	m_TextureImage.Cleanup(device);
	m_Descriptor->Cleanup(device);
	for (size_t i{ 0 }; i < m_MaxFrameInFlight; i++)
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
	m_UniformBuffer.resize(m_MaxFrameInFlight);
	m_UniformBuffersMapped.resize(m_MaxFrameInFlight);
	for (size_t i{}; i < m_MaxFrameInFlight; ++i)
	{
		constexpr VkDeviceSize bufferSize = sizeof(UniformBufferObject);
		DataBuffer::CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffer[i].GetBuffer(), m_UniformBuffer[i].GetBufferMemory());
		vkMapMemory(device, m_UniformBuffer[i].GetBufferMemory(), 0, bufferSize, 0, &m_UniformBuffersMapped[i]);
	}
}

void Shader::CreateDescriptorSets(VkDevice device, VkDescriptorSetLayout descriptorLayout, const TextureImage& textureImage) 
{
	m_Descriptor->CreateDescriptorSets(device, descriptorLayout, m_UniformBuffer, textureImage);
}

void Shader::CreateDescriptorSetLayout(VkDevice device)
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // For image sampling

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	VkResult result{ vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_DescriptorSetLayout) };
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}
