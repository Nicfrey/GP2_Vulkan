#pragma once
#include "Mesh2D.h"
class CircleMesh2D : public Mesh2D
{
public:
	CircleMesh2D() = default;
	CircleMesh2D(const glm::vec2& center, float radius, uint32_t segments);
	CircleMesh2D(const glm::vec2& center, float radiusX, float radiusY, uint32_t segments);
	CircleMesh2D(const glm::vec2& center, float radius, uint32_t segments, const glm::vec3& color);
	CircleMesh2D(const glm::vec2& center, float radiusX, float radiusY, uint32_t segments, const glm::vec3& color);
	~CircleMesh2D() override = default;
	void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,
		VkQueue graphicQueue, VkDescriptorSetLayout descriptorSetLayout) override;
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const override;
	void Cleanup(VkDevice device) const override;
};