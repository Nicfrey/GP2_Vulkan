#include "Helper/Camera.h"

#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Helper.h"
#include "MathHelpers.h"
#include "GLFW/glfw3.h"

Camera::Camera(const glm::vec3& position, float fovAngle)
{
	m_Position = position;
	m_FOV = tanf((fovAngle * MathHelper::TO_RADIANS) / 2.f);
}

void Camera::Init(float fovAngle, const glm::vec3& position, float aspectRatio)
{
	m_FOVAngle = fovAngle;
	m_FOV = tanf((fovAngle * MathHelper::TO_RADIANS) / 2.f);

	m_Position = position;
	m_Aspect = aspectRatio;

	CalculateViewMatrix();
	CalculateProjectionMatrix();
}

void Camera::Update(VkExtent2D swapchainExtent)
{
	m_Aspect = static_cast<float>(swapchainExtent.width) / static_cast<float>(swapchainExtent.height);
	CalculateViewMatrix();
	CalculateProjectionMatrix();
}

void Camera::SetIsDragging(bool isDragging)
{
	m_IsDragging = isDragging;
}

bool Camera::GetIsDragging() const
{
	return m_IsDragging;
}

void Camera::MoveCamera(float x, float y)
{
	if (m_IsDragging)
	{
		const float newX{ x - m_LastMousePosition.x };
		const float newY{ y - m_LastMousePosition.y };
		if (newX != 0.f)
		{
			const float xSign{ -newX / std::abs(newX) };
			const float xNormalized{ newX / (newX / std::abs(newX)) };
			m_TotalYaw += Helper::TimerVulkan::GetDeltaTime() * m_Sensitivity * xSign * xNormalized;
		}

		if (newY != 0.f)
		{
			const float ySign{ -newY / std::abs(newY) };
			const float yNormalized{ newY / (newY / std::abs(newY)) };
			m_TotalPitch += Helper::TimerVulkan::GetDeltaTime() * m_Sensitivity * ySign * yNormalized;
		}

		m_LastMousePosition = { x,y };

		if (m_TotalPitch > 89.f)
		{
			m_TotalPitch = 89.f;
		}
		if (m_TotalPitch < -89.f)
		{
			m_TotalPitch = -89.f;
		}
	}
}

void Camera::CalculateViewMatrix()
{
	glm::mat4 rotation{rotate(glm::mat4(1.f), glm::radians(m_TotalYaw), {0, 1, 0})};
	rotation = rotate(rotation, glm::radians(m_TotalPitch), {1, 0, 0});

	m_Forward = normalize(glm::vec3(rotation * glm::vec4(0, 0, -1, 0)));
	m_Right = normalize(glm::cross(m_Forward, {0, 1, 0}));
	m_ViewMatrix = lookAt(m_Position, m_Position + m_Forward, {0, 1, 0});
}

void Camera::CalculateProjectionMatrix()
{
	m_ProjectionMatrix = glm::perspective(m_FOV, m_Aspect, m_Near, m_Far);
}

glm::mat4 Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

glm::mat4 Camera::GetWorldViewProjectionMatrix() const
{
	return m_ProjectionMatrix * m_ViewMatrix;
}

glm::vec3 Camera::GetPosition() const
{
	return m_Position;
}

void Camera::HandleKeyInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_Position += m_Forward * m_Speed * Helper::TimerVulkan::GetDeltaTime();
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_Position -= m_Forward * m_Speed * Helper::TimerVulkan::GetDeltaTime();
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_Position -= m_Right * m_Speed * Helper::TimerVulkan::GetDeltaTime();
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_Position += m_Right * m_Speed * Helper::TimerVulkan::GetDeltaTime();
	}
}

float Camera::GetFOV() const
{
	return m_FOV;
}

float Camera::GetNear() const
{
	return m_Near;
}

float Camera::GetFar() const
{
	return m_Far;
}
