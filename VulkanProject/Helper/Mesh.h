#pragma once
#include <vector>
#include "Vertex.h"
#include "VertexBuffer.h"

class Mesh
{
public:
	Mesh() = default;
	virtual ~Mesh() = default;
	void AddVertex(const Vertex2D& vertex);
	void AddVertex(const glm::vec2& position, const glm::vec3& color);
	void AddVertex(const glm::vec2& position);
	void AddIndex(uint32_t index);
	size_t GetVerticesSizeInByte() const;
	size_t GetVerticesSize() const;
	size_t GetIndicesSizeInByte() const;
	size_t GetIndicesSize() const;
	void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue);
	void Draw(VkCommandBuffer commandBuffer) const;
	void Cleanup(VkDevice device) const;
private:
	std::vector<uint32_t> m_Indices;
	std::vector<Vertex2D> m_Vertices;

	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
};
