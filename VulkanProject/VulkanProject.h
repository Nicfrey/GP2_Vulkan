#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#define SELECTING_DEVICE
#include <optional>
#include <string>
#include <vector>

#include "Helper/Helper.h"
#include "Helper/VertexBuffer.h"
#include "Helper/Pipeline.h"
#include "Helper/Scene.h"
#include "Helper/TextureImage.h"

#ifdef NDEBUG
const bool enableValidationLayers{ false };
#else
class VertexBuffer;
const bool enableValidationLayers{ true };
#endif


struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete() const
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

const std::vector<const char*> validationLayers =
{
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator);
void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

class VulkanApp
{
public:
	VulkanApp();
	void Run();
	static VkImageView CreateImageView(const VkDevice& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
	static std::vector<char> ReadFile(const std::string& filename);
private:
	void HandleCursorPos(GLFWwindow* window, double xpos, double ypos);


	GLFWwindow* m_Window;
	uint32_t m_Width{ 800 };
	uint32_t m_Height{ 600 };
	// instance
	VkInstance m_Instance;
	VkDebugUtilsMessengerEXT m_DebugMessenger;
	VkSurfaceKHR m_Surface;
	VkPhysicalDevice m_PhysicalDevice{ VK_NULL_HANDLE };
	VkDevice m_Device;
	VkQueue m_GraphicsQueue;
	VkQueue m_PresentQueue;
	VkSwapchainKHR m_SwapChain;
	std::vector<VkImage> m_SwapChainImages;
	VkFormat m_SwapChainImageFormat;
	VkExtent2D m_SwapChainExtent;
	std::vector<VkFramebuffer> m_SwapChainFramebuffers;

	std::vector<VkImageView> m_SwapChainImageViews;
	VkRenderPass m_RenderPass;

	TextureImage m_TextureImage;
	Pipeline<Vertex2D> m_Pipeline2D;
	Pipeline<Vertex3D> m_Pipeline3D;
	Pipeline<Vertex3D> m_PipelinePBR;

	VkCommandPool m_CommandPool;
	std::vector<VkCommandBuffer> m_CommandBuffers;

	std::vector<VkSemaphore> m_ImageAvailableSemaphores;
	std::vector<VkSemaphore> m_RenderFinishedSemaphores;
	std::vector<VkFence> m_InFlightFences;
	uint32_t m_CurrentFrame{ 0 };
	bool m_FramebufferResized{ false };

	Scene m_Scene2D{};
	Scene m_Scene3D{};
	TextureImage textureImage{"TestTexture.jpg"};

	VkImage m_DepthImage;
	VkDeviceMemory m_DepthImageMemory;
	VkImageView m_DepthImageView;
	Camera m_Camera;

	void SetupDebugMessenger();
	Scene* InitSceneJSON();
	void InitVulkan();
	void MainLoop();
	void Cleanup();
	void InitWindow();
	void Update();
	void CreateInstance();
	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();
	/**
	 * Select a physical device to use with Vulkan
	*/
	void PickPhysicalDevice();
	/**
	 * Check if a physical device is suitable for the application (e.g. GPU)
	*/
	bool IsDeviceSuitable(VkPhysicalDevice device);
	/**
	 * Rate a physical device based on its suitability for the application
	*/
	// int RateDeviceSuitability(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	void CreateLogicalDevice();
	void CreateSurface();
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateSwapChain();
	void CreateImageViews();
	void CreateRenderPass();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffer();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void DrawFrame();
	void CreateSyncObjects();
	/**
	 * Handle when the framebuffer is resized
	 */
	void RecreateSwapChain();
	/**
	 * Cleanup the swap chain
	 */
	void CleanupSwapChain();
	static void FrameBufferReziseCallback(GLFWwindow* window, int width, int height);
	void CreateDepthResources();
	VkFormat FindDepthFormat();
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	static bool HasStencilComponent(VkFormat format);
	Helper::TimerVulkan m_Timer;
};
