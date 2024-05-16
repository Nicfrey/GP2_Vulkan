#include "Helper/CircleMesh2D.h"

CircleMesh2D::CircleMesh2D(const glm::vec2& center, float radius, uint32_t segments): CircleMesh2D{center, radius,radius,segments}
{
}

CircleMesh2D::CircleMesh2D(const glm::vec2& center, float radiusX, float radiusY, uint32_t segments): CircleMesh2D{center, radiusX, radiusY, segments, glm::vec3(1.f)}
{
	
}

CircleMesh2D::CircleMesh2D(const glm::vec2& center, float radius, uint32_t segments, const glm::vec3& color) : CircleMesh2D{ center, radius, radius, segments, color }
{
}

CircleMesh2D::CircleMesh2D(const glm::vec2& center, float radiusX, float radiusY, uint32_t segments,
	const glm::vec3& color)
{
	AddVertex(center, color);
	constexpr float pi{ 3.14159f };
	const float angle{ 2 * pi / segments };
	for (uint32_t i{ 0 }; i < segments; ++i)
	{
		const float x = center.x + radiusX * std::cosf(i * angle);
		const float y = center.y + radiusY * std::sinf(i * angle);
		AddVertex(glm::vec2(x, y), color);
	}

	for (uint32_t i{ 0 }; i < segments; ++i)
	{
		AddIndex(0);
		AddIndex(i + 1);
		AddIndex((i + 1) % segments + 1);
	}
}
