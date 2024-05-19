#pragma once
#include <glm/mat4x4.hpp>
#include <vulkan/vulkan.h>

#include "GLFW/glfw3.h"


class Camera
{
public:
	Camera() = default;
	Camera(const glm::vec3& position, float fovAngle);
	void Init(float fovAngle, const glm::vec3& position, float aspectRatio);
	void Update(VkExtent2D swapchainExtent);
	void SetIsDragging(bool isDragging);
	bool GetIsDragging() const;
	void MoveCamera(float x, float y);

	void CalculateViewMatrix();
	void CalculateProjectionMatrix();
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetWorldViewProjectionMatrix() const;
	glm::vec3 GetPosition() const;
	void HandleKeyInput(GLFWwindow* window);
	float GetFOV() const;
	float GetNear() const;
	float GetFar() const;

private:
	glm::vec3 m_Position{};

	glm::vec3 m_Up{glm::vec3{0.f,1.f,0.f}};
	glm::vec3 m_Right{glm::vec3{1.f,0.f,0.f}};
	glm::vec3 m_Forward{glm::vec3{0.f,0.f,1.f}};

	float m_TotalPitch{};
	float m_TotalYaw{};

	float m_FOV{};
	float m_FOVAngle{};
	float m_Aspect{};
	float m_Near{1.f};
	float m_Far{1000.f};

	glm::mat4 m_ViewMatrix{};
	glm::mat4 m_ProjectionMatrix{};

	bool m_IsDragging{};
	float m_Sensitivity{1.f};
	float m_Speed{100.f};

	glm::vec2 m_LastMousePosition{};
};
