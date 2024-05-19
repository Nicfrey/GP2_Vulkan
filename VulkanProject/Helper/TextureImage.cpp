#include "Helper/TextureImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

#include "Helper.h"
#include "VertexBuffer.h"

TextureImage::TextureImage(std::string path): m_TexturePath{std::move(path)}
{
}

TextureImage::TextureImage(const std::string& path, VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool,VkQueue graphicsQueue)
{
	Init(device, physicalDevice, commandPool, graphicsQueue);
}

void TextureImage::Init(VkDevice device, VkPhysicalDevice physicalDevice,
	VkCommandPool commandPool, VkQueue graphicsQueue)
{
	int textWidth;
	int textHeight;
	int textChannels;
	const std::string newPath{ m_DataPath + m_TexturePath };
	stbi_uc* pixels{ stbi_load(newPath.c_str(),&textWidth,&textHeight,&textChannels,STBI_rgb_alpha) };
	VkDeviceSize imageSize = textWidth * textHeight * 4;
	if (!pixels)
	{
		throw std::runtime_error("Failed to load texture image:" + m_TexturePath);
	}

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	DataBuffer::CreateBuffer(device, physicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(device, stagingBufferMemory);

	stbi_image_free(pixels);

	CreateImage(textWidth, textHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_TextureImage, m_TextureImageMemory, device, physicalDevice);
	TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, device, commandPool, graphicsQueue);
	CopyBufferToImage(stagingBuffer, m_TextureImage, static_cast<uint32_t>(textWidth), static_cast<uint32_t>(textHeight), device, commandPool, graphicsQueue);
	TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, device, commandPool, graphicsQueue);
	m_TextureImageView = CreateImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, device);
	CreateTextureSampler(m_TextureSampler, device);
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void TextureImage::Cleanup(VkDevice device) const
{
	vkDestroySampler(device, m_TextureSampler, nullptr);
	vkDestroyImageView(device, m_TextureImageView, nullptr);
	vkDestroyImage(device, m_TextureImage, nullptr);
	vkFreeMemory(device, m_TextureImageMemory, nullptr);
}

void TextureImage::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                               VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory,
                               VkDevice device, VkPhysicalDevice physicalDevice)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D; // What kind of coordinate system to use
	imageInfo.extent.width = width; // Width of the image
	imageInfo.extent.height = height; // Height of the image
	imageInfo.extent.depth = 1; // How many texels on each axis
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling; // How texels should be laid out in memory
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Layout of the image data on creation
	imageInfo.usage = usage; // What the image is going to be used for
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // If the image can be shared between queues
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT; // Number of samples for multisampling

	VkResult result{ vkCreateImage(device,&imageInfo,nullptr,&image) };
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create image");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = DataBuffer::FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

	result = vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate image memory");
	}

	vkBindImageMemory(device, image, imageMemory, 0);
}

VkImageView TextureImage::CreateImageView(VkImage image, VkFormat format,VkImageAspectFlags aspectFlags, VkDevice device)
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image; // Image to create view for
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // Type of image
	viewInfo.format = format; // Format of image data
	viewInfo.subresourceRange.aspectMask = aspectFlags; // Which aspect of the image to view
	viewInfo.subresourceRange.baseMipLevel = 0; // Start mipmap level
	viewInfo.subresourceRange.levelCount = 1; // Number of mipmap levels
	viewInfo.subresourceRange.baseArrayLayer = 0; // Start array layer
	viewInfo.subresourceRange.layerCount = 1; // Number of array layers

	VkImageView imageView;
	VkResult result{ vkCreateImageView(device,&viewInfo,nullptr,&imageView) };
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create texture image view");
	}
	return imageView;
}

void TextureImage::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
                                         VkImageLayout newLayout, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	VkCommandBuffer commandBuffer(DataBuffer::BeginSingleTimeCommands(device, commandPool));

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER; // Type of the structure
	barrier.oldLayout = oldLayout; // Layout to transition from
	barrier.newLayout = newLayout; // Layout to transition to
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // Queue family to transition from
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // Queue family to transition to
	barrier.image = image; // Image to transition
	if(newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		if(Helper::HasStencilComponent(format))
		{
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // Which aspect of the image to transition
	}
	barrier.subresourceRange.baseMipLevel = 0; // Mipmap level to start the transition
	barrier.subresourceRange.levelCount = 1; // Number of mipmap levels to transition
	barrier.subresourceRange.baseArrayLayer = 0; // Start array layer for the transition
	barrier.subresourceRange.layerCount = 1; // Number of array layers to transition

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0; // Stage of the pipeline to transition from
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; // Stage of the pipeline to transition to

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT; 
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT; 
	}
	else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; 
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT; 

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT; 
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT; 
	}
	else if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else
	{
		throw std::invalid_argument("Unsupported layout transition");
	}

	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	DataBuffer::EndSingleTimeCommands(commandBuffer, device, graphicsQueue, commandPool);
}

void TextureImage::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkDevice device,
	VkCommandPool commandPool, VkQueue graphicsQueue)
{
	VkCommandBuffer commandBuffer(DataBuffer::BeginSingleTimeCommands(device, commandPool));

	VkBufferImageCopy region{};
	region.bufferOffset = 0; // Offset into the buffer
	region.bufferRowLength = 0; // How the data is laid out in the buffer
	region.bufferImageHeight = 0; // How the data is laid out in the buffer
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // Which aspect of the image to copy
	region.imageSubresource.mipLevel = 0; // Mipmap level
	region.imageSubresource.baseArrayLayer = 0; // Start array layer
	region.imageSubresource.layerCount = 1; // Number of array layers
	region.imageOffset = { 0,0,0 }; // Offset into the image
	region.imageExtent = { width,height,1 }; // Size of the image

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	DataBuffer::EndSingleTimeCommands(commandBuffer, device, graphicsQueue, commandPool);
}

void TextureImage::CreateTextureSampler(VkSampler& sampler, VkDevice device)
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR; // How to interpolate texels that are magnified
	samplerInfo.minFilter = VK_FILTER_LINEAR; // How to interpolate texels that are minified
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT; // How to handle addressing mode in U
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT; // How to handle addressing mode in V
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT; // How to handle addressing mode in W
	samplerInfo.anisotropyEnable = VK_FALSE; // Enable anisotropic filtering
	samplerInfo.maxAnisotropy = 1.f; // Anisotropy level
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK; // Border color
	samplerInfo.unnormalizedCoordinates = VK_FALSE; // Whether coordinates are normalized
	samplerInfo.compareEnable = VK_FALSE; // Enable comparison function
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS; // Comparison function
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR; // Mipmap mode
	samplerInfo.mipLodBias = 0.0f; // Level of detail bias for mipmap level
	samplerInfo.minLod = 0.0f; // Minimum level of detail
	samplerInfo.maxLod = 0.0f; // Maximum level of detail

	const VkResult result{ vkCreateSampler(device,&samplerInfo,nullptr,&sampler) };
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create texture sampler");
	}
}
