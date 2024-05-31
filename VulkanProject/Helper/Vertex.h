#pragma once
#include <array>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include  "vulkan/vulkan.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

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
	glm::vec3 normal;
	glm::vec3 tangent;

	bool operator==(const Vertex3D& other) const
	{
		return pos == other.pos && color == other.color && textCoord == other.textCoord;
	}

	static VkVertexInputBindingDescription GetBinding();

	static std::array<VkVertexInputAttributeDescription, 5> GetAttributeDescriptions();
};


	template<> struct std::hash<Vertex3D>
	{
		size_t operator()(Vertex3D const& vertex) const noexcept
		{
			return ((std::hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.textCoord) << 1);
		}
	};

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

struct Constants
{
	glm::vec3 lightDir{ 0.555f,-0.577f,0.577f };
	float lightIntensity{ 7.f };
	glm::vec3 cameraPos{};
	int useAlbedo{ 1 };
	glm::vec3 albedoValue{ 1.f,1.f,1.f };
	int useNormal{ 1 };
	int useRoughness{ 1 };
	float roughnessValue{ 0.5f };
	int useMetal{ 1 };
	float metalValue{ 0.5f };
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

inline std::array<VkVertexInputAttributeDescription, 5> Vertex3D::GetAttributeDescriptions()
{
	std::array<VkVertexInputAttributeDescription, 5> attributeDescriptions{};
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

	attributeDescriptions[3].binding = 0;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[3].offset = offsetof(Vertex3D, normal);

	attributeDescriptions[4].binding = 0;
	attributeDescriptions[4].location = 4;
	attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[4].offset = offsetof(Vertex3D, tangent);

	return attributeDescriptions;
}
