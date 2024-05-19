#pragma once
#include <vector>
#include "Vertex.h"
#include "VertexBuffer.h"

class Mesh
{
public:
	Mesh() = default;
	virtual ~Mesh() = default;
	void AddIndex(uint32_t index);
	size_t GetIndicesSizeInByte() const;
	size_t GetIndicesSize() const;
	virtual void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, VkDescriptorSetLayout descriptorSetLayout);
	virtual void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const;
	virtual void Cleanup(VkDevice device) const;
protected:
	std::vector<uint32_t> m_Indices;
private:
	IndexBuffer m_IndexBuffer;
};
