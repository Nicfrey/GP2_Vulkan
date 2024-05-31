#pragma once
#include <string>

#include "Mesh3D.h"
#include "tiny_obj_loader.h"


class MeshObj : public Mesh3D
{
public:
	MeshObj(const std::string& file);
	~MeshObj() override = default;

	// Load only triangular meshes
	static bool LoadObj(const std::string& filename, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, bool flipAxisAndWinding = true);
	static void CalculateTangent(std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices);
	static void GenerateNormals(tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes);
	void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue,
		VkDescriptorSetLayout descriptorLayout) override;
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const override;
	void Cleanup(VkDevice device) const override;

private:
	static std::string m_ResourceFolder;
};
