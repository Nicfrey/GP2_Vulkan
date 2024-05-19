#pragma once
#include <chrono>
#include <vulkan/vulkan.h>

namespace Helper
{
	constexpr int MAX_FRAMES_IN_FLIGHT{ 2 };

	inline bool HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	class TimerVulkan final
	{
	public:
		TimerVulkan() = default;
		~TimerVulkan() = default;

		TimerVulkan(const TimerVulkan& other) = delete;
		TimerVulkan(TimerVulkan&& other) noexcept = delete;
		TimerVulkan& operator=(const TimerVulkan& other) = delete;
		TimerVulkan& operator=(TimerVulkan&& other) noexcept = delete;
		void Start();
		void Update();
		static float GetDeltaTime();
	private:
		static float m_DeltaTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_CurrentTime;
	};

	inline void TimerVulkan::Start()
	{
		m_LastTime = std::chrono::high_resolution_clock::now();
	}

	inline void TimerVulkan::Update()
	{
		m_CurrentTime = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<float> duration = m_CurrentTime - m_LastTime;
		m_DeltaTime = duration.count();
		m_LastTime = m_CurrentTime;
	}

	inline float TimerVulkan::GetDeltaTime()
	{
		return m_DeltaTime;
	}
}
