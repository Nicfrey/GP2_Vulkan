#pragma once
#include <string>
#include <vector>

#include "vulkan/vulkan.h"

class Pipeline
{
public:
	Pipeline() = default;
	Pipeline(VkDevice device, VkExtent2D swapChainExtent, VkRenderPass renderPass, const std::string& fileVertex,
		const std::string& fileFragment);
	void Cleanup(VkDevice device);
	VkPipeline& GetPipeline() { return m_GraphicsPipeline; }
private:

	static std::vector<char> ReadFile(const std::string& filename);
	static VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code);
	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;
};
