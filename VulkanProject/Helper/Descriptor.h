#pragma once
#include <iosfwd>
#include <memory>
#include <vector>
#include <vector>

#include "TextureImage.h"
#include "VertexBuffer.h"
#include "vulkan/vulkan.h"

class Descriptor
{
public:
	Descriptor(VkDevice device);
	~Descriptor() = default;
	VkDescriptorPool GetDescriptorPool() const;

	Descriptor(const Descriptor& other) = delete;
	Descriptor(Descriptor&& other) noexcept = delete;
	Descriptor& operator=(const Descriptor& other) = delete;
	Descriptor& operator=(Descriptor&& other) noexcept = delete;

	void BindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t currentFrame) const;
	void Cleanup(VkDevice device) const;
	void CreateDescriptorSets(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, std::vector<DataBuffer>& buffers, const std::unique_ptr<
	                          TextureImage>& textureImage);
private:
	std::vector<VkDescriptorSet> m_DescriptorSets;
	VkDescriptorPool m_DescriptorPool;
};