#pragma once
#include <memory>
#include <vector>

#include "TextureImage.h"
#include "VertexBuffer.h"
#include "vulkan/vulkan.h"

class Descriptor;

class Shader
{
public:
	Shader() = default;
	~Shader() = default;
	Shader(const Shader& other) = delete;
	Shader(Shader&& other) noexcept = delete;
	Shader& operator=(const Shader& other) = delete;
	Shader& operator=(Shader&& other) noexcept = delete;

	VkDescriptorSetLayout& GetDescriptorSetLayout();
	void Initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkCommandPool& commandPool,
	                const VkQueue& graphicsQueue,
	                const int& maxFrameInFlight);
	void Update(uint32_t currentFrame, float deltaTime);
	void Cleanup(const VkDevice& device);
	void BindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t currentFrame);
	void CreateDescriptorSets(VkDevice device, VkDescriptorSetLayout descriptorLayout, const TextureImage& textureImage);
	void CreateDescriptorSetLayout(VkDevice device);
private:
	std::unique_ptr<Descriptor> m_Descriptor{};
	std::vector<DataBuffer> m_UniformBuffer{};
	std::vector<void*> m_UniformBuffersMapped{};
	VkDescriptorSetLayout m_DescriptorSetLayout{};
	TextureImage m_TextureImage{};
	int m_MaxFrameInFlight{};

	void CreateUniformBuffers(VkPhysicalDevice physicalDevice, VkDevice device);
};
