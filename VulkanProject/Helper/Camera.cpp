#include "Helper/Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "MathHelpers.h"

Camera::Camera(const glm::vec3& position, float fovAngle)
{
	m_Position = position;
	m_FOV = tanf((fovAngle * TO_RADIANS) / 2.f);
}

void Camera::Init(float fovAngle, const glm::vec3& position, float aspectRatio)
{
	m_FOVAngle = fovAngle;
	m_FOV = tanf((fovAngle * TO_RADIANS) / 2.f);

	m_Position = position;
	m_Aspect = aspectRatio;

	CalculateViewMatrix();
	CalculateProjectionMatrix();
}

void Camera::Update()
{
	CalculateViewMatrix();
	CalculateProjectionMatrix();
}

void Camera::CalculateViewMatrix()
{
	glm::mat4 rotation{};
	rotation = rotate(rotation, glm::radians(m_TotalPitch), { 1.f,0.f,0.f });
	rotation = rotate(rotation, glm::radians(m_TotalYaw), {0.f, 1.f, 0.f});
	rotation = rotate(rotation, 0.f, {0.f, 0.f, 1.f});

	const glm::mat4 translation{ glm::translate(glm::mat4(1.f), m_Position) };

	const glm::mat4 view{ rotation * translation };

	m_ViewMatrix = inverse(view);

	glm::vec4 up{ 0.f,1.f,0.f,1.f };
	m_Forward = view * up;
	m_Forward = normalize(m_Forward);
	glm::vec4 right{ 1.f,0.f,0.f,1.f };
	m_Right = view * right;
	m_Right = normalize(m_Right);
}

void Camera::CalculateProjectionMatrix()
{
	m_ProjectionMatrix = glm::perspective(glm::radians(m_FOVAngle), m_Aspect, m_Near, m_Far);
}

glm::mat4 Camera::GetWorldViewProjectionMatrix() const
{
	return m_ProjectionMatrix * m_ViewMatrix;
}

glm::vec3 Camera::GetPosition() const
{
	return m_Position;
}
