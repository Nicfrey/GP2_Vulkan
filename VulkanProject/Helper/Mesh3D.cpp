#include "Helper/Mesh3D.h"

#include <stdexcept>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

void Mesh3D::Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, VkDescriptorSetLayout descriptorLayout)
{
	const VkDeviceSize vertexBufferSize{ GetVerticesSizeInByte() };
	m_VertexBuffer = VertexBuffer(physicalDevice, device, commandPool, graphicQueue, vertexBufferSize, m_Vertices.data());
	Mesh::Init(physicalDevice, device, commandPool, graphicQueue, descriptorLayout);
}

void Mesh3D::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const
{
	m_VertexBuffer.BindVertexBuffer(commandBuffer);
	Mesh::Draw(commandBuffer, currentFrame, pipelineLayout);
}

void Mesh3D::Update(uint32_t currentImage, float deltaTime)
{
	// TODO
}

void Mesh3D::Cleanup(VkDevice device) const
{
	m_VertexBuffer.Cleanup(device);
	Mesh::Cleanup(device);
}

void Mesh3D::AddVertex(const Vertex3D& vertex)
{
	m_Vertices.push_back(vertex);
}

auto Mesh3D::AddVertex(const glm::vec3& position, const glm::vec3& color, const glm::vec2& textCoord) -> void
{
	m_Vertices.push_back({ position, color, textCoord });
}

size_t Mesh3D::GetVerticesSizeInByte() const
{
	return m_Vertices.size() * sizeof(Vertex3D);
}

size_t Mesh3D::GetVerticesSize() const
{
	return m_Vertices.size();
}
