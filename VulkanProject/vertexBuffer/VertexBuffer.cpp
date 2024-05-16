#include "vertexBuffer/VertexBuffer.h"

VkVertexInputBindingDescription Vertex::GetBinding()
{
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 2> Vertex::GetAttributeDescriptions()
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
	attributeDescriptions[0].offset = offsetof(Vertex, pos);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);
	return attributeDescriptions;
}
