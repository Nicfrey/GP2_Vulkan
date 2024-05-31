#pragma once
#include "Mesh3D.h"

class CubeMesh : public Mesh3D
{
public:
	CubeMesh(const glm::vec3& position, float size);
	~CubeMesh() override = default;
private:
	void CreateCube(float size);
};
