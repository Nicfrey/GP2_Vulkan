#pragma once
#include "Mesh.h"

class Mesh2D : public Mesh
{
public:
	Mesh2D() = default;
	~Mesh2D() override = default;
	void AddVertex(const Vertex2D& vertex);
	void AddVertex(const glm::vec2& position, const glm::vec3& color);
	void AddVertex(const glm::vec2& position);
	size_t GetVerticesSizeInByte() const;
	size_t GetVerticesSize() const;
	void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,
		VkQueue graphicQueue,VkDescriptorSetLayout descriptorSetLayout) override;
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const override;
	void Cleanup(VkDevice device) const override;
protected:
	std::vector<Vertex2D> m_Vertices;
private:
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
};