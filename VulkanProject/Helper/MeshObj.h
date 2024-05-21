#pragma once
#include <string>

#include "Mesh3D.h"


class MeshObj : public Mesh3D
{
public:
	MeshObj(const std::string& file);
	~MeshObj() override = default;

	static bool LoadObj(const std::string& filename, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, bool flipAxisAndWinding = true);
	static void CalculateTangent(std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices);
	void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue,
		VkDescriptorSetLayout descriptorLayout) override;
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const override;
	void Cleanup(VkDevice device) const override;

private:
	static std::string m_ResourceFolder;
};