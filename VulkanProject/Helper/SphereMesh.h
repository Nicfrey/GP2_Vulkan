#pragma once
#include "Mesh3D.h"

class SphereMesh : public Mesh3D
{
public:
	SphereMesh(const glm::vec3& center, float radius, int nbStacks, int nbSlices);
	~SphereMesh() override = default;
private:
	void CreateSphere(const glm::vec3& center, float radius, int nbStacks, int nbSlices);
};