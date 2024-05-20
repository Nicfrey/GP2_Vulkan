#include "CubeMesh.h"

#include <stdexcept>

#include "MeshObj.h"
CubeMesh::CubeMesh()
{
	if (!MeshObj::LoadObj("cube.obj",m_Vertices,m_Indices))
	{
		throw std::runtime_error("Failed to load cube mesh");
	}
}

