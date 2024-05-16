#pragma once
#include "Helper/Mesh.h"
class CircleMesh2D : public Mesh
{
public:
	CircleMesh2D() = default;
	CircleMesh2D(const glm::vec2& center, float radius, uint32_t segments);
	CircleMesh2D(const glm::vec2& center, float radiusX, float radiusY, uint32_t segments);
	CircleMesh2D(const glm::vec2& center, float radius, uint32_t segments, const glm::vec3& color);
	CircleMesh2D(const glm::vec2& center, float radiusX, float radiusY, uint32_t segments, const glm::vec3& color);
};