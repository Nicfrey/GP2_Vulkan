#pragma once
#include <fstream>
#include <string>
#include <vector>

#include "Descriptor.h"
#include "Scene.h"
#include "Shader.h"
#include "vulkan/vulkan.h"


class Shader;

template<typename T>
struct is_vertex : std::false_type {};

template<>
struct is_vertex<Vertex2D> : std::true_type {};

template<>
struct is_vertex<Vertex3D> : std::true_type {};

template<typename T, typename = std::enable_if_t<is_vertex<T>::value>>
class Pipeline
{
public:
	Pipeline() = default;
	void InitializePipeline(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D swapChainExtent, VkRenderPass renderPass,
	                        VkCommandPool commandPool, VkQueue graphicsQueue, const std::string& fileVertex, const std::string& fileFragment, Scene
	                        * scene, size_t
	                        frames);
	void InitScene(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue) const;
	void Cleanup(VkDevice device);
	void DrawFrame(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkExtent2D swapChainExtent) const;
	VkPipeline& GetPipeline() { return m_GraphicsPipeline; }
	VkPipelineLayout& GetPipelineLayout() { return m_PipelineLayout; }
	void Update(uint32_t currentFrame, float deltaTime);
	void CreateDescriptorSets(VkDevice device, const TextureImage& textureImage) const;

private:
	static std::vector<char> ReadFile(const std::string& filename);
	static VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code);
	std::unique_ptr<Shader> m_Shader;
	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;
	Scene* m_pScene;
};

template <typename T, typename T0>
void Pipeline<T, T0>::InitializePipeline(VkDevice device,VkPhysicalDevice physicalDevice,VkExtent2D swapChainExtent, VkRenderPass renderPass, VkCommandPool commandPool, VkQueue graphicsQueue,
	const std::string& fileVertex, const std::string& fileFragment, Scene* scene, size_t frames)
{
	m_Shader = std::make_unique<Shader>();
	m_Shader->Initialize(device, physicalDevice, commandPool, graphicsQueue, static_cast<int>(frames));
	m_pScene = scene;
	auto vertShaderCode{ ReadFile(fileVertex) };
	auto fragShaderCode{ ReadFile(fileFragment) };

	VkShaderModule vertShaderModule{ CreateShaderModule(device,vertShaderCode) };
	VkShaderModule fragShaderModule{ CreateShaderModule(device,fragShaderCode) };

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	// Dynamic State
	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

	// Vertex Input
	auto bindingDescription{ T::GetBinding() };
	auto attributeDescriptions{ T::GetAttributeDescriptions() }; // TODO Need to change


	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	// Input Assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// ViewPorts and scissors
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;

	// Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo{};
	rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerCreateInfo.depthClampEnable = VK_FALSE;
	rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizerCreateInfo.lineWidth = 1.0f;
	if(typeid(T) == typeid(Vertex2D))
	{
		rasterizerCreateInfo.cullMode = VK_CULL_MODE_NONE;
		rasterizerCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	}
	else
	{
		rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizerCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	}
	
	rasterizerCreateInfo.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo{};
	multisamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingCreateInfo.sampleShadingEnable = VK_FALSE;
	multisamplingCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlendingCreateInfo{};
	colorBlendingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendingCreateInfo.logicOpEnable = VK_FALSE;
	colorBlendingCreateInfo.attachmentCount = 1;
	colorBlendingCreateInfo.pAttachments = &colorBlendAttachment;

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &m_Shader->GetDescriptorSetLayout();

	VkResult result{ vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout) };
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stageCount = 2;
	pipelineCreateInfo.pStages = shaderStages;
	pipelineCreateInfo.pVertexInputState = &vertexInputInfo;
	pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
	pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	pipelineCreateInfo.pRasterizationState = &rasterizerCreateInfo;
	pipelineCreateInfo.pMultisampleState = &multisamplingCreateInfo;
	pipelineCreateInfo.pDepthStencilState = nullptr;
	pipelineCreateInfo.pColorBlendState = &colorBlendingCreateInfo;
	pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
	pipelineCreateInfo.layout = m_PipelineLayout;
	pipelineCreateInfo.renderPass = renderPass;
	pipelineCreateInfo.subpass = 0;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex = -1;

	result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_GraphicsPipeline);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

template <typename T, typename T0>
void Pipeline<T, T0>::InitScene(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,
	VkQueue graphicQueue) const
{
	m_pScene->Init(physicalDevice, device, commandPool, graphicQueue, m_Shader->GetDescriptorSetLayout());
}

template <typename T, typename T0>
void Pipeline<T, T0>::Cleanup(VkDevice device)
{
	m_Shader->Cleanup(device);
	vkDestroyPipeline(device, m_GraphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
	m_pScene->Cleanup(device);
	delete m_pScene;
	m_pScene = nullptr;
}

template <typename T, typename T0>
void Pipeline<T, T0>::DrawFrame(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkExtent2D swapChainExtent) const
{
	m_Shader->BindDescriptorSets(commandBuffer, m_PipelineLayout, currentFrame);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer
		, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	// Draw scene
	m_pScene->Draw(commandBuffer, currentFrame, m_PipelineLayout);
}

template <typename T, typename T0>
void Pipeline<T, T0>::Update(uint32_t currentFrame, float deltaTime)
{
	m_Shader->Update(currentFrame, deltaTime);
	m_pScene->Update(currentFrame, deltaTime);
}

template <typename T, typename T0>
void Pipeline<T, T0>::CreateDescriptorSets(VkDevice device, const TextureImage& textureImage) const
{
	m_Shader->CreateDescriptorSets(device, m_Shader->GetDescriptorSetLayout(),textureImage);

}

template <typename T, typename T0>
std::vector<char> Pipeline<T, T0>::ReadFile(const std::string& filename)
{
	std::ifstream file{ filename, std::ios::ate | std::ios::binary };
	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file: " + filename);
	}

	// Read at the end of the file to determine the file size
	const size_t fileSize{ static_cast<size_t>(file.tellg()) };
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();
	return buffer;
}

template <typename T, typename T0>
VkShaderModule Pipeline<T, T0>::CreateShaderModule(VkDevice device, const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	const VkResult result{ vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) };
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}
