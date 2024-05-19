#pragma once
#include "Helper/Mesh.h"

class Mesh3D : public Mesh
{
public:
	Mesh3D() = default;
	~Mesh3D() override = default;
	void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,
		VkQueue graphicQueue, VkDescriptorSetLayout descriptorLayout) override;
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const override;
	void Update(uint32_t currentImage, float deltaTime);
	void Cleanup(VkDevice device) const override;
	void AddVertex(const Vertex3D& vertex);
	void AddVertex(const glm::vec3& position, const glm::vec3& color, const glm::vec2& textCoord);
	size_t GetVerticesSizeInByte() const;
	size_t GetVerticesSize() const;
protected:
	std::vector<Vertex3D> m_Vertices{};
	VertexBuffer m_VertexBuffer{};
	IndexBuffer m_IndexBuffer{};
};