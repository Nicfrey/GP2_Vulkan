#include "Helper/Mesh3D.h"

#include <stdexcept>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Camera.h"
#include "imgui.h"
#include "Shader.h"

void Mesh3D::SetPosition(const glm::vec3& position)
{
	m_Position = position;
	// Update Vertex
	/*
	for (auto& vertex : m_Vertices)
	{
		vertex.pos += m_Position;
	}
	*/
}

glm::vec3 Mesh3D::GetPosition() const
{
	return m_Position;
}

void Mesh3D::SetTextureImage(const std::string& path)
{
	if (path.empty())
	{
		m_TextureImage = std::make_unique<TextureImage>("defaultAlbedo.png");
		return;
	}
	m_TextureImage = std::make_unique<TextureImage>(path);
}

void Mesh3D::SetTextureNormal(const std::string& path)
{
	if (path.empty())
	{
		m_TextureNormal = std::make_unique<TextureImage>("defaultNormal.png");
		return;
	}
	m_TextureNormal = std::make_unique<TextureImage>(path);
}

void Mesh3D::SetTextureRoughness(const std::string& path)
{
	if(path.empty())
	{
		m_TextureRoughness = std::make_unique<TextureImage>("defaultRoughness.png");
		return;
	}
	m_TextureRoughness = std::make_unique<TextureImage>(path);
}

void Mesh3D::SetTextureSpecular(const std::string& path)
{
	if (path.empty())
	{
		m_TextureMetallic = std::make_unique<TextureImage>("defaultMetallic.png");
		return;
	}
	m_TextureMetallic = std::make_unique<TextureImage>(path);
}

void Mesh3D::Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, VkDescriptorSetLayout descriptorLayout)
{
	m_TextureImage->Init(device, physicalDevice, commandPool, graphicQueue);
	m_TextureNormal->Init(device, physicalDevice, commandPool, graphicQueue);
	m_TextureRoughness->Init(device, physicalDevice, commandPool, graphicQueue);
	m_TextureMetallic->Init(device, physicalDevice, commandPool, graphicQueue);
	m_Shader = std::make_unique<Shader>();
	m_Shader->Initialize(device,physicalDevice);
	m_Shader->CreateDescriptorSets(device, descriptorLayout, m_TextureImage, m_TextureNormal, m_TextureRoughness, m_TextureMetallic);
	const VkDeviceSize vertexBufferSize{ GetVerticesSizeInByte() };
	m_VertexBuffer = VertexBuffer(physicalDevice, device, commandPool, graphicQueue, vertexBufferSize, m_Vertices.data());
	Mesh::Init(physicalDevice, device, commandPool, graphicQueue, descriptorLayout);
}

void Mesh3D::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const
{
	m_Shader->BindDescriptorSets(commandBuffer, pipelineLayout, currentFrame);
	m_VertexBuffer.BindVertexBuffer(commandBuffer);

	vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(Constants), &m_Constants);

	Mesh::Draw(commandBuffer, currentFrame, pipelineLayout);
}

void Mesh3D::Update(uint32_t currentImage, float deltaTime, VkExtent2D swapchainExtent, const Camera& camera)
{
	m_Constants.cameraPos = camera.GetPosition();
	m_Shader->Update(currentImage, deltaTime,swapchainExtent, camera, GetPosition());
}

void Mesh3D::Cleanup(VkDevice device) const
{
	m_TextureImage->Cleanup(device);
	m_TextureNormal->Cleanup(device);
	m_TextureRoughness->Cleanup(device);
	m_TextureMetallic->Cleanup(device);
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

VkDescriptorPool Mesh3D::GetDescriptorPool() const
{
	return m_Shader->GetDescriptorPool();
}

void Mesh3D::SetAlbedoValue(const glm::vec3& albedo)
{
	m_Constants.albedoValue = albedo;
}

void Mesh3D::SetRoughnessValue(float roughness)
{
	m_Constants.roughnessValue = roughness;
}

void Mesh3D::SetMetallicValue(float metallic)
{
	m_Constants.metalValue = metallic;
}

void Mesh3D::UseAlbedoMap(bool use)
{
	m_Constants.useAlbedo = use;
}

void Mesh3D::UseNormalMap(bool use)
{
	m_Constants.useNormal = use;
}

void Mesh3D::UseRoughnessMap(bool use)
{
	m_Constants.useRoughness = use;
}

void Mesh3D::UseMetalMap(bool use)
{
	m_Constants.useMetal = use;
}
