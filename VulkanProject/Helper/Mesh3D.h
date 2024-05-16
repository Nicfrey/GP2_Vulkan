#pragma once
#include "Helper/Mesh.h"
class Mesh3D : public Mesh
{
public:
	Mesh3D(int maxFrameInFlight);
	~Mesh3D() override = default;
	void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,
		VkQueue graphicQueue, VkDescriptorSetLayout descriptorLayout) override;
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const override;
	void Update(uint32_t currentImage, float deltaTime);
	void Cleanup(VkDevice device) const override;

private:
	int m_MaxFrameInFlight;
	std::vector<VkBuffer> m_UniformBuffers{};
	std::vector<VkDeviceMemory> m_UniformBuffersMemory{};
	VertexBuffer m_VertexBuffer{};
	IndexBuffer m_IndexBuffer{};
	std::vector<void*> m_UniformBuffersMapped{};
	VkDescriptorPool m_DescriptorPool{};
	std::vector<VkDescriptorSet> m_DescriptorSets;

	void CreateUniformBuffers(VkPhysicalDevice physicalDevice, VkDevice device);
};