#pragma once
#include <array>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class VkVertexInputBindingDescription;
class VkVertexInputAttributeDescription;

struct Vertex
{
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription GetBinding();

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
};
