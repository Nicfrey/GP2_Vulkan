#pragma once
#include <array>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include  "vulkan/vulkan.h"

struct Vertex2D
{
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription GetBinding();

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
};

struct Vertex3D
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 textCoord;

	static VkVertexInputBindingDescription GetBinding();

	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
};

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

const std::vector<Vertex2D> vertices = {
	{{-0.5f,-0.5f}, {1.f,0.f,0.f}},
	{{0.5f,-0.5f},{0.f,1.f,0.f}},
	{{0.5f,0.5f},{0.f,0.f,1.f}},
	{{-0.5f,0.5f},{1.f,1.f,1.f}}
};

const std::vector<uint32_t> indices{
	0,1,2,2,3,0
};

inline VkVertexInputBindingDescription Vertex2D::GetBinding()
{
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex2D);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescription;
}

inline std::array<VkVertexInputAttributeDescription, 2> Vertex2D::GetAttributeDescriptions()
{
	std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
	attributeDescriptions[0].binding = 0;
	// The location (layout (location = 0)) 
	attributeDescriptions[0].location = 0;
	/*
	 * float : VK_FORMAT_R32_SFLOAT
	 * double : VK_FORMAT_R64_SFLOAT
	 * vec2 : VK_FORMAT_R32G32_SFLOAT
	 * vec3 : VK_FORMAT_R32G32B32_SFLOAT
	 * vec4 : VK_FORMAT_R32G32B32A32_SFLOAT
	 * ivec2 : VK_FORMAT_R32G32_SINT
	 * uvec4 : VK_FORMAT_R32G32B32A32_UINT
	 */
	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex2D, pos);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex2D, color);

	return attributeDescriptions;
}

inline VkVertexInputBindingDescription Vertex3D::GetBinding()
{
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex3D);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescription;
}

inline std::array<VkVertexInputAttributeDescription, 3> Vertex3D::GetAttributeDescriptions()
{
	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
	attributeDescriptions[0].binding = 0;
	// The location (layout (location = 0)) 
	attributeDescriptions[0].location = 0;
	/*
	 * float : VK_FORMAT_R32_SFLOAT
	 * double : VK_FORMAT_R64_SFLOAT
	 * vec2 : VK_FORMAT_R32G32_SFLOAT
	 * vec3 : VK_FORMAT_R32G32B32_SFLOAT
	 * vec4 : VK_FORMAT_R32G32B32A32_SFLOAT
	 * ivec2 : VK_FORMAT_R32G32_SINT
	 * uvec4 : VK_FORMAT_R32G32B32A32_UINT
	 */
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex3D, pos);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex3D, color);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Vertex3D, textCoord);
	return attributeDescriptions;
}
