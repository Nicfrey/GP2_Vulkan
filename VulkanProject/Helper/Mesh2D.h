#pragma once
#include "Mesh.h"

class Mesh2D : public Mesh
{
public:
	Mesh2D() = default;
	~Mesh2D() override = default;
	void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,
		VkQueue graphicQueue,VkDescriptorSetLayout descriptorSetLayout) override;
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const override;
	void Cleanup(VkDevice device) const override;

private:
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
};