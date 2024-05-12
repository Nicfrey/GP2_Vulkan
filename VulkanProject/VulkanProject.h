#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vector>

#include "GLFW/glfw3.h"
#include "vulkan/vulkan.h"

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator);
void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

class VulkanApp
{
public:
	void Run();
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
private:
	GLFWwindow* m_Window;
	uint32_t m_Width{ 800 };
	uint32_t m_Height{ 600 };
	// instance
	VkInstance m_Instance;
	VkDebugUtilsMessengerEXT m_DebugMessenger;

	void SetupDebugMessenger();
	void InitVulkan();
	void MainLoop();
	void Cleanup();
	void CreateInstance();
	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();
	/**
	 * Select a physical device to use in the application (e.g. a GPU).
	 * If there are no GPUs that support Vulkan, the application will exit.
	 */
	void PickPhysicalDevice();
	/**
	 * Check if a physical device is suitable for the application.
	 * @param device the device to check
	 * @return true if the device is suitable, false otherwise
	 */
	bool IsDeviceSuitable(VkPhysicalDevice device);
};
