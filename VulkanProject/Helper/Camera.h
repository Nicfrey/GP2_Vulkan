#pragma once
#include <glm/mat4x4.hpp>

class Camera
{
public:
	Camera() = default;
	Camera(const glm::vec3& position, float fovAngle);
	void Init(float fovAngle, const glm::vec3& position, float aspectRatio);
	void Update();

	void CalculateViewMatrix();
	void CalculateProjectionMatrix();
	glm::mat4 GetWorldViewProjectionMatrix() const;
	glm::vec3 GetPosition() const;
private:
	glm::vec3 m_Position{};

	glm::vec3 m_Up{};
	glm::vec3 m_Right{};
	glm::vec3 m_Forward{};

	float m_TotalPitch{};
	float m_TotalYaw{};

	float m_FOV{};
	float m_FOVAngle{};
	float m_Aspect{};
	float m_Near{1.f};
	float m_Far{1000.f};

	glm::mat4 m_ViewMatrix{};
	glm::mat4 m_ProjectionMatrix{};
};
