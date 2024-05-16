#include "Helper/RectangleMesh2D.h"

// White color
RectangleMesh2D::RectangleMesh2D(const glm::vec2& bottomLeft, float width, float height) : RectangleMesh2D(bottomLeft, width, height, glm::vec3(1.f))
{
}

RectangleMesh2D::RectangleMesh2D(const glm::vec2& bottomLeft, float width, float height, const glm::vec3& color)
{
	AddVertex(bottomLeft, color);
	AddVertex(bottomLeft + glm::vec2(width, 0.f), color);
	AddVertex(bottomLeft + glm::vec2(width, height), color);
	AddVertex(bottomLeft + glm::vec2(0.f, height), color);

	AddIndex(0);
	AddIndex(1);
	AddIndex(2);
	AddIndex(2);
	AddIndex(3);
	AddIndex(0);
}

void RectangleMesh2D::Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,
	VkQueue graphicQueue, VkDescriptorSetLayout descriptorSetLayout)
{
	Mesh2D::Init(physicalDevice, device, commandPool, graphicQueue,descriptorSetLayout);
}

void RectangleMesh2D::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const
{
	Mesh2D::Draw(commandBuffer, currentFrame, pipelineLayout);
}

void RectangleMesh2D::Cleanup(VkDevice device) const
{
	Mesh2D::Cleanup(device);
}
