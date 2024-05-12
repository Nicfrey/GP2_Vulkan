#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
//#define SELECTING_DEVICE 
#include <vector>

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
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
	VkSurfaceKHR m_Surface;
	VkPhysicalDevice m_PhysicalDevice{ VK_NULL_HANDLE };
	VkDevice m_Device;
	VkQueue m_GraphicsQueue;

	void SetupDebugMessenger();
	void InitVulkan();
	void MainLoop();
	void Cleanup();
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
	int RateDeviceSuitability(VkPhysicalDevice device);
	uint32_t FindQueueFamilies(VkPhysicalDevice device);
	void CreateLogicalDevice();
	void CreateSurface();
};
