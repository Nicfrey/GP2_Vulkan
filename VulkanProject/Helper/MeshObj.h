#pragma once
#include <string>

#include "Mesh3D.h"


class MeshObj : public Mesh3D
{
public:
	MeshObj(const std::string& file);
	~MeshObj() override = default;
	static bool LoadObj(const std::string& filename, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, bool flipAxisAndWinding = true);
private:
	static std::string m_ResourceFolder;
};