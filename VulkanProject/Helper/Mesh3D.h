#pragma once
#include <memory>

#include "Shader.h"
#include "TextureImage.h"
#include "Helper/Mesh.h"

class Mesh3D : public Mesh
{
public:
	Mesh3D() = default;
	~Mesh3D() override = default;
	void SetPosition(const glm::vec3& position);
	glm::vec3 GetPosition() const;
	void SetTextureImage(const std::string& path);
	void SetTextureNormal(const std::string& path);
	void SetTextureRoughness(const std::string& path);
	void SetTextureSpecular(const std::string& path);
	void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,
		VkQueue graphicQueue, VkDescriptorSetLayout descriptorLayout) override;
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const override;
	virtual void Update(uint32_t currentImage, float deltaTime, VkExtent2D swapchainExtent, const Camera& camera);
	void Cleanup(VkDevice device) const override;
	void AddVertex(const Vertex3D& vertex);
	void AddVertex(const glm::vec3& position, const glm::vec3& color, const glm::vec2& textCoord);
	size_t GetVerticesSizeInByte() const;
	size_t GetVerticesSize() const;
protected:
	std::vector<Vertex3D> m_Vertices{};
	VertexBuffer m_VertexBuffer{};
	std::unique_ptr<Shader> m_Shader{};
	std::unique_ptr<TextureImage> m_TextureImage{ std::make_unique<TextureImage>("defaultAlbedo.png")};
	Constants m_Constants{};
	std::unique_ptr<TextureImage> m_TextureNormal{ std::make_unique<TextureImage>("defaultNormal.png") };
	std::unique_ptr<TextureImage> m_TextureRoughness{ std::make_unique<TextureImage>("defaultRoughness.png")};
	std::unique_ptr<TextureImage> m_TextureMetallic{ std::make_unique<TextureImage>("defaultMetallic.png")};

private:
	glm::vec3 m_Position{};
};
