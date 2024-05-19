#include "Helper/Mesh3D.h"

#include <stdexcept>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Mesh3D::Mesh3D()
{
	AddVertex( {-0.5f,-0.5f}, {1.f,0.f,0.f} , {1.0f,0.f} );
	AddVertex( {0.5f,-0.5f},{0.f,1.f,0.f},{0.f,0.f} );
	AddVertex( {0.5f,0.5f},{0.f,0.f,1.f},{0.f,1.f} );
	AddVertex( {-0.5f,0.5f},{1.f,1.f,1.f}, {1.f,1.f} );
	AddIndex(0);
	AddIndex(1);
	AddIndex(2);
	AddIndex(2);
	AddIndex(3);
	AddIndex(0);
}

void Mesh3D::Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, VkDescriptorSetLayout descriptorLayout)
{
	const VkDeviceSize vertexBufferSize{ GetVerticesSizeInByte() };
	m_VertexBuffer = VertexBuffer(physicalDevice, device, commandPool, graphicQueue, vertexBufferSize, m_Vertices.data());

	const VkDeviceSize indexBufferSize{ GetIndicesSizeInByte() };
	m_IndexBuffer = IndexBuffer(physicalDevice, device, commandPool, graphicQueue, indexBufferSize, m_Indices.data());
}

void Mesh3D::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const
{
	m_VertexBuffer.BindVertexBuffer(commandBuffer);
	m_IndexBuffer.BindIndexBuffer(commandBuffer, VK_INDEX_TYPE_UINT32);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);
}

void Mesh3D::Update(uint32_t currentImage, float deltaTime)
{
	// TODO
}

void Mesh3D::Cleanup(VkDevice device) const
{
	m_VertexBuffer.Cleanup(device);
	m_IndexBuffer.Cleanup(device);
}

void Mesh3D::AddVertex(const Vertex3D& vertex)
{
	m_Vertices.push_back(vertex);
}

void Mesh3D::AddVertex(const glm::vec2& position, const glm::vec3& color)
{
	m_Vertices.push_back({ position, color });
}

void Mesh3D::AddVertex(const glm::vec2& position)
{
	m_Vertices.push_back({ position, {1.f,1.f,1.f} });
}

void Mesh3D::AddVertex(const glm::vec2& position, const glm::vec3& color, const glm::vec2& textCoord)
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
