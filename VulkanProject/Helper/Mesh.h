#pragma once
#include <vector>
#include "Vertex.h"

class Mesh
{
public:
	Mesh() = default;
	virtual ~Mesh() = default;
private:
	std::vector<uint32_t> m_Indices;
	std::vector<Vertex> m_Vertices;
};