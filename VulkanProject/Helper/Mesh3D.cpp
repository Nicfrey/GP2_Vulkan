#include "Helper/Mesh3D.h"

#include <stdexcept>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Camera.h"
#include "Shader.h"

void Mesh3D::SetPosition(const glm::vec3& position)
{
	m_Position = position;
	// Update Vertex
	for (auto& vertex : m_Vertices)
	{
		vertex.pos += m_Position;
	}
}

glm::vec3 Mesh3D::GetPosition() const
{
	return m_Position;
}

void Mesh3D::SetTextureImage(const std::string& path)
{
	m_TextureImage = std::make_unique<TextureImage>(path);
}

void Mesh3D::Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, VkDescriptorSetLayout descriptorLayout)
{
	m_TextureImage->Init(device, physicalDevice, commandPool, graphicQueue);
	m_Shader = std::make_unique<Shader>();
	m_Shader->Initialize(device,physicalDevice);
	m_Shader->CreateDescriptorSets(device, descriptorLayout, m_TextureImage);
	const VkDeviceSize vertexBufferSize{ GetVerticesSizeInByte() };
	m_VertexBuffer = VertexBuffer(physicalDevice, device, commandPool, graphicQueue, vertexBufferSize, m_Vertices.data());
	Mesh::Init(physicalDevice, device, commandPool, graphicQueue, descriptorLayout);
}

void Mesh3D::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const
{
	m_Shader->BindDescriptorSets(commandBuffer, pipelineLayout, currentFrame);
	m_VertexBuffer.BindVertexBuffer(commandBuffer);

	vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(CameraConstants), &m_CameraConstants);

	Mesh::Draw(commandBuffer, currentFrame, pipelineLayout);
}

void Mesh3D::Update(uint32_t currentImage, float deltaTime, VkExtent2D swapchainExtent, const Camera& camera)
{
	m_CameraConstants = { camera.GetPosition() };
	m_Shader->Update(currentImage, deltaTime,swapchainExtent, camera);
}

void Mesh3D::Cleanup(VkDevice device) const
{
	m_TextureImage->Cleanup(device);
	m_Shader->Cleanup(device);
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
