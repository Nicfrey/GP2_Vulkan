#pragma once
#include <memory>
#include <vector>

#include "Descriptor.h"
#include "TextureImage.h"
#include "VertexBuffer.h"
#include "vulkan/vulkan.h"

class Shader
{
public:
	Shader() = default;
	~Shader() = default;
	Shader(const Shader& other) = delete;
	Shader(Shader&& other) noexcept = delete;
	Shader& operator=(const Shader& other) = delete;
	Shader& operator=(Shader&& other) noexcept = delete;

	void Initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice);
	void Update(uint32_t currentFrame, float deltaTime, VkExtent2D swapchainExtent);
	void Cleanup(const VkDevice& device);
	void BindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t currentFrame);
	void CreateDescriptorSets(VkDevice device, VkDescriptorSetLayout descriptorLayout, const std::unique_ptr<TextureImage>& textureImage);
private:
	std::unique_ptr<Descriptor> m_Descriptor{};
	std::vector<DataBuffer> m_UniformBuffer{};
	std::vector<void*> m_UniformBuffersMapped{};
	float m_Test{ 0 };

	void CreateUniformBuffers(VkPhysicalDevice physicalDevice, VkDevice device);
};
