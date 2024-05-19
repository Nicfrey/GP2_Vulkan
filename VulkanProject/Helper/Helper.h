#pragma once
#include <vulkan/vulkan.h>

namespace Helper
{
	constexpr int MAX_FRAMES_IN_FLIGHT{ 2 };

	inline bool HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}
}