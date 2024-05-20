#include "MeshObj.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>

#include "MathHelpers.h"

std::string MeshObj::m_ResourceFolder = "resources/";

MeshObj::MeshObj(const std::string& file)
{
	if (!LoadObj(file, m_Vertices, m_Indices))
	{
		throw std::runtime_error("Failed to load obj file");
	}
}

bool MeshObj::LoadObj(const std::string& filename, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices,
                      bool flipAxisAndWinding)
{
	std::string fullPath{ m_ResourceFolder + filename };
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fullPath.c_str())) {
		throw std::runtime_error(err);
	}

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex3D vertex{};
			vertex.pos = {
							attrib.vertices[3 * index.vertex_index + 0],
							attrib.vertices[3 * index.vertex_index + 1],
							attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.textCoord = {
							attrib.texcoords[2 * index.texcoord_index + 0],
							1.f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			if (!attrib.normals.empty())
			{
				vertex.normal = {
								attrib.normals[3 * index.normal_index + 0],
								attrib.normals[3 * index.normal_index + 1],
								attrib.normals[3 * index.normal_index + 2]
				};
			}
			

			vertex.color = { 1.f, 1.f, 1.f };

			vertices.push_back(vertex);
			indices.push_back(static_cast<uint32_t>(indices.size()));
		}
	}
	return true;
}

void MeshObj::Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue,
                   VkDescriptorSetLayout descriptorLayout)
{
	Mesh3D::Init(physicalDevice, device, commandPool, graphicQueue, descriptorLayout);
}

void MeshObj::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const
{
	Mesh3D::Draw(commandBuffer, currentFrame, pipelineLayout);
}

void MeshObj::Cleanup(VkDevice device) const
{
	Mesh3D::Cleanup(device);
}
