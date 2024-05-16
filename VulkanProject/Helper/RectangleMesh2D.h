#pragma once
#include "Mesh2D.h"
class RectangleMesh2D : public Mesh2D
{
public:
	RectangleMesh2D() = default;
	RectangleMesh2D(const glm::vec2& bottomLeft, float width, float height);
	RectangleMesh2D(const glm::vec2& bottomLeft, float width, float height, const glm::vec3& color);
	~RectangleMesh2D() override = default;
	void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,
		VkQueue graphicQueue, VkDescriptorSetLayout descriptorSetLayout) override;
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const override;
	void Cleanup(VkDevice device) const override;
};