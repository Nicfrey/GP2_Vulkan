#pragma once
#include <fstream>
#include <string>
#include <vector>

#include "Descriptor.h"
#include "Scene.h"
#include "vulkan/vulkan.h"
#include "Camera.h"



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
	void Update(uint32_t currentFrame, float deltaTime, VkExtent2D swapchainExtent, const Camera& camera);

private:
	static std::vector<char> ReadFile(const std::string& filename);
	static VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code);
	void CreateDescriptorSetLayout(VkDevice device);
	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;
	VkDescriptorSetLayout m_DescriptorSetLayout;
	Scene* m_pScene;
};

template <typename T, typename T0>
void Pipeline<T, T0>::InitializePipeline(VkDevice device,VkPhysicalDevice physicalDevice,VkExtent2D swapChainExtent, VkRenderPass renderPass, VkCommandPool commandPool, VkQueue graphicsQueue,
	const std::string& fileVertex, const std::string& fileFragment, Scene* scene, size_t frames)
{
	m_pScene = scene;
	CreateDescriptorSetLayout(device);
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
	auto attributeDescriptions{ T::GetAttributeDescriptions() };

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

	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(Constants);

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

	VkResult result{ vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout) };
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

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
	pipelineCreateInfo.pDepthStencilState = &depthStencil;

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
	m_pScene->Init(physicalDevice, device, commandPool, graphicQueue, m_DescriptorSetLayout);
}

template <typename T, typename T0>
void Pipeline<T, T0>::Cleanup(VkDevice device)
{
	vkDestroyDescriptorSetLayout(device, m_DescriptorSetLayout, nullptr);
	vkDestroyPipeline(device, m_GraphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
	m_pScene->Cleanup(device);
	delete m_pScene;
	m_pScene = nullptr;
}

template <typename T, typename T0>
void Pipeline<T, T0>::DrawFrame(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkExtent2D swapChainExtent) const
{
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
void Pipeline<T, T0>::Update(uint32_t currentFrame, float deltaTime, VkExtent2D swapchainExtent, const Camera& camera)
{
	m_pScene->Update(currentFrame, deltaTime, swapchainExtent, camera);
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

template <typename T, typename T0>
void Pipeline<T, T0>::CreateDescriptorSetLayout(VkDevice device)
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // For image sampling

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;


	VkDescriptorSetLayoutBinding samplerNormalBinding{};
	samplerNormalBinding.binding = 2;
	samplerNormalBinding.descriptorCount = 1;
	samplerNormalBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerNormalBinding.pImmutableSamplers = nullptr;
	samplerNormalBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding samplerRoughnessBinding{};
	samplerRoughnessBinding.binding = 3;
	samplerRoughnessBinding.descriptorCount = 1;
	samplerRoughnessBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerRoughnessBinding.pImmutableSamplers = nullptr;
	samplerRoughnessBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding samplerSpecularBinding{};
	samplerSpecularBinding.binding = 4;
	samplerSpecularBinding.descriptorCount = 1;
	samplerSpecularBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerSpecularBinding.pImmutableSamplers = nullptr;
	samplerSpecularBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 5> bindings = { uboLayoutBinding, samplerLayoutBinding, samplerNormalBinding, samplerRoughnessBinding, samplerSpecularBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	VkResult result{ vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_DescriptorSetLayout) };
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}
