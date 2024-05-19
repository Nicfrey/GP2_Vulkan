#pragma once
#include <string>

#include "vulkan/vulkan.h"

class TextureImage
{
public:
	TextureImage() = default;
	TextureImage(std::string path);
	~TextureImage() = default;
	TextureImage(const std::string& path, VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool,
	             VkQueue graphicsQueue);

	void Init(std::string path, VkDevice device, VkPhysicalDevice physicalDevice,
	          VkCommandPool commandPool, VkQueue graphicsQueue);
	void Cleanup(VkDevice device) const;
	bool HasTexture() const { return !m_TexturePath.empty(); }
	VkImageView GetTextureImageView() const { return m_TextureImageView; }
	VkSampler GetTextureSampler() const { return m_TextureSampler; }
	static void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkDevice device, VkPhysicalDevice physicalDevice);
	static VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkDevice device);
	static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
	static void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
	static void CreateTextureSampler(VkSampler& sampler, VkDevice device);
private:
	const std::string m_DataPath{"textures/"};
	std::string m_TexturePath;
	VkImage m_TextureImage;
	VkDeviceMemory m_TextureImageMemory;
	VkImageView m_TextureImageView;
	VkSampler m_TextureSampler;
};
