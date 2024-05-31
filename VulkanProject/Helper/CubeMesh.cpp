#include "CubeMesh.h"

#include <stdexcept>

#include "MeshObj.h"

CubeMesh::CubeMesh(const glm::vec3& position, float size)
{
	SetPosition(position);
	CreateCube(size);
	MeshObj::CalculateTangent(m_Vertices, m_Indices);
}

void CubeMesh::CreateCube(float size)
{
	m_Vertices.clear();
	m_Indices.clear();

	const float halfSize = size / 2.f;
	AddVertex(Vertex3D{ { -size, -size, size }, glm::vec3{ 1.f },  { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } });
	AddVertex({ center.x + (0.5f * width), center.y - (0.5f * height), center.z + 0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f });
	AddVertex({ center.x + (0.5f * width), center.y + (0.5f * height), center.z + 0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f });
	AddVertex({ -size, center.y + (0.5f * height), center.z + 0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f });

	//t1
	AddIndex(2);
	AddIndex(1);
	AddIndex(0);

	//t2
	AddIndex(0);
	AddIndex(3);
	AddIndex(2);

	//Left face
	AddVertex({ center.x + (0.5f * width), center.y - (0.5f * height), center.z - 0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ center.x + (0.5f * width), center.y - (0.5f * height), center.z + 0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ center.x + (0.5f * width), center.y + (0.5f * height), center.z - 0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ center.x + (0.5f * width), center.y + (0.5f * height), center.z + 0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f });

	//t1
	AddIndex(4);
	AddIndex(5);
	AddIndex(6);

	//t2
	AddIndex(7);
	AddIndex(6);
	AddIndex(5);

	//Right face
	AddVertex({ center.x - (0.5f * width), center.y - (0.5f * height), center.z - 0.5f }, { 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ center.x - (0.5f * width), center.y - (0.5f * height), center.z + 0.5f }, { 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ center.x - (0.5f * width), center.y + (0.5f * height), center.z - 0.5f }, { 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ center.x - (0.5f * width), center.y + (0.5f * height), center.z + 0.5f }, { 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f });

	//t1
	AddIndex(10);
	AddIndex(9);
	AddIndex(8);

	//t2
	AddIndex(9);
	AddIndex(10);
	AddIndex(11);

	//Top face
	AddVertex({ center.x - (0.5f * width), center.y + (0.5f * height), center.z - 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f });
	AddVertex({ center.x - (0.5f * width), center.y + (0.5f * height), center.z + 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f });
	AddVertex({ center.x + (0.5f * width), center.y + (0.5f * height), center.z - 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f });
	AddVertex({ center.x + (0.5f * width), center.y + (0.5f * height), center.z + 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f });

	//t1
	AddIndex(14);
	AddIndex(13);
	AddIndex(12);

	//t2
	AddIndex(13);
	AddIndex(14);
	AddIndex(15);

	//Bottom face
	AddVertex({ center.x - (0.5f * width), center.y - (0.5f * height), center.z - 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f });
	AddVertex({ center.x - (0.5f * width), center.y - (0.5f * height), center.z + 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f });
	AddVertex({ center.x + (0.5f * width), center.y - (0.5f * height), center.z - 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f });
	AddVertex({ center.x + (0.5f * width), center.y - (0.5f * height), center.z + 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f });

	//t1
	AddIndex(16);
	AddIndex(17);
	AddIndex(18);

	//t2
	AddIndex(19);
	AddIndex(18);
	AddIndex(17);

	//BottomFace
	AddVertex({ center.x - (0.5f * width), center.y - (0.5f * height), center.z - 0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f });
	AddVertex({ center.x + (0.5f * width), center.y - (0.5f * height), center.z - 0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f });
	AddVertex({ center.x + (0.5f * width), center.y + (0.5f * height), center.z - 0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f });
	AddVertex({ center.x - (0.5f * width), center.y + (0.5f * height), center.z - 0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f });

	//t1
	AddIndex(20);
	AddIndex(21);
	AddIndex(22);

	//t2
	AddIndex(20);
	AddIndex(22);
	AddIndex(23);
}

