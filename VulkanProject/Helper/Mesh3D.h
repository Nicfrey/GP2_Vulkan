#pragma once
#include "TextureImage.h"
#include "Helper/Mesh.h"
class Mesh3D : public Mesh
{
public:
	Mesh3D(int maxFrameInFlight, const std::string& texture = "");
	~Mesh3D() override = default;
	void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,
		VkQueue graphicQueue, VkDescriptorSetLayout descriptorLayout) override;
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const override;
	void Update(uint32_t currentImage, float deltaTime);
	void Cleanup(VkDevice device) const override;
	void AddVertex(const Vertex3D& vertex);
	void AddVertex(const glm::vec2& position, const glm::vec3& color);
	void AddVertex(const glm::vec2& position);
	void AddVertex(const glm::vec2& position, const glm::vec3& color, const glm::vec2& textCoord);
	size_t GetVerticesSizeInByte() const;
	size_t GetVerticesSize() const;
private:
	std::vector<Vertex3D> m_Vertices{};
	int m_MaxFrameInFlight;
	std::vector<VkBuffer> m_UniformBuffers{};
	std::vector<VkDeviceMemory> m_UniformBuffersMemory{};
	VertexBuffer m_VertexBuffer{};
	IndexBuffer m_IndexBuffer{};
	std::vector<void*> m_UniformBuffersMapped{};
	VkDescriptorPool m_DescriptorPool{};
	std::vector<VkDescriptorSet> m_DescriptorSets;
	TextureImage m_TextureImage{};

	void CreateUniformBuffers(VkPhysicalDevice physicalDevice, VkDevice device);
};