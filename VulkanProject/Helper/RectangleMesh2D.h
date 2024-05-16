#pragma once
#include "Helper/Mesh.h"
class RectangleMesh2D : public Mesh
{
public:
	RectangleMesh2D() = default;
	RectangleMesh2D(const glm::vec2& bottomLeft, float width, float height);
	RectangleMesh2D(const glm::vec2& bottomLeft, float width, float height, const glm::vec3& color);
};