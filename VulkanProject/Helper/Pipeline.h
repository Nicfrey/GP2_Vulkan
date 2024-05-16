#pragma once
#include <string>
#include <vector>

#include "Scene.h"
#include "vulkan/vulkan.h"

class Pipeline
{
public:
	Pipeline() = default;
	Pipeline(VkDevice device, VkExtent2D swapChainExtent, VkRenderPass renderPass, const std::string& fileVertex,
		const std::string& fileFragment, VkDescriptorSetLayout descriptorLayout, Scene* scene);
	void InitScene(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, VkDescriptorSetLayout
		descriptorSetLayout) const;
	void Cleanup(VkDevice device);
	void DrawFrame(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkExtent2D swapChainExtent) const;
	VkPipeline& GetPipeline() { return m_GraphicsPipeline; }
	VkPipelineLayout& GetPipelineLayout() { return m_PipelineLayout; }
	void Update(uint32_t currentFrame, float deltaTime);

private:

	static std::vector<char> ReadFile(const std::string& filename);
	static VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code);
	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;
	Scene* m_pScene;
};
