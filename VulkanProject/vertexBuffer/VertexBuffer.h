#pragma once
#include <array>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include  "vulkan/vulkan.h"

struct Vertex
{
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription GetBinding();

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
};

const std::vector<Vertex> vertices = {
	{{-0.5f,-0.5f}, {1.f,0.f,0.f}},
	{{0.5f,-0.5f},{0.f,1.f,0.f}},
	{{0.5f,0.5f},{0.f,0.f,1.f}},
	{{-0.5f,0.5f},{1.f,1.f,1.f}}
};

const std::vector<uint32_t> indices{
	0,1,2,2,3,0
};