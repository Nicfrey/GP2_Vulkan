#include "CubeMesh.h"

#include "MeshObj.h"

CubeMesh::CubeMesh(const glm::vec3& position, float size)
{
	SetPosition(position);
	CreateCube(size);
	// MeshObj::CalculateTangent(m_Vertices, m_Indices);
}

void CubeMesh::CreateCube(float size)
{
	const float halfSize{size / 2.f};

	std::vector<Vertex3D> vertices{
		{{-halfSize, -halfSize, halfSize}, glm::vec3{1.f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.f,0.f,0.f}},
		{{halfSize, -halfSize, halfSize}, glm::vec3{1.f}, {1.0f, 0.0f}, {0.0f, 0.0f,1.0f}, {1.f,0.f,0.f}},
		{{halfSize, halfSize, halfSize}, glm::vec3{1.f}, {1.0f, 1.0f}, {0.0f, 0.f, 1.0f}, {1.f,0.f,0.f}},
		{{-halfSize, halfSize, halfSize}, glm::vec3{1.f}, {0.0f, 1.0f}, {0.0f, 0.0f,1.0f}, {1.f,0.f,0.f}},

		{{halfSize, -halfSize, -halfSize}, glm::vec3{1.f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.f,0.f,1.f}},
		{{halfSize, -halfSize, halfSize}, glm::vec3{1.f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.f,0.f,1.f}},
		{{halfSize, halfSize, -halfSize}, glm::vec3{1.f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.f,0.f,1.f}},
		{{halfSize, halfSize, halfSize}, glm::vec3{1.f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.f,0.f,1.f}},

		{{-halfSize, -halfSize, -halfSize}, glm::vec3{1.f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.f,0.f,1.f}},
		{{-halfSize, -halfSize, halfSize}, glm::vec3{1.f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.f,0.f,1.f}},
		{{-halfSize, halfSize, -halfSize}, glm::vec3{1.f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.f,0.f,1.f}},
		{{-halfSize, halfSize, halfSize}, glm::vec3{1.f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.f,0.f,1.f}},


		{{-halfSize, halfSize, -halfSize}, glm::vec3{1.f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.f,0.f,0.f}},
		{{-halfSize, halfSize, halfSize}, glm::vec3{1.f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.f,0.f,0.f}},
		{{halfSize, halfSize, -halfSize}, glm::vec3{1.f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.f,0.f,0.f}},
		{{halfSize, halfSize, halfSize}, glm::vec3{1.f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.f,0.f,0.f}},

		{{-halfSize, -halfSize, -halfSize}, glm::vec3{1.f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.f,0.f,0.f}},
		{{-halfSize, -halfSize, halfSize}, glm::vec3{1.f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.f,0.f,0.f}},
		{{halfSize, -halfSize, -halfSize}, glm::vec3{1.f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.f,0.f,0.f}},
		{{halfSize, -halfSize, halfSize}, glm::vec3{1.f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.f,0.f,0.f}},

		{{-halfSize, -halfSize, -halfSize}, glm::vec3{1.f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {-1.f,0.f,0.f}},
		{{halfSize, -halfSize, -halfSize}, glm::vec3{1.f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {-1.f,0.f,0.f}},
		{{halfSize, halfSize, -halfSize}, glm::vec3{1.f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {-1.f,0.f,0.f}},
		{{-halfSize, halfSize, -halfSize}, glm::vec3{1.f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {-1.f,0.f,0.f}}
	};

	std::vector<uint32_t> indices{
		2, 1, 0, 0, 3, 2,
		4, 5, 6, 7, 6, 5,
		10, 9, 8, 9, 10, 11,
		14, 13, 12, 13, 14, 15,
		16, 17, 18, 19, 18, 17,
		20, 21, 22, 20, 22, 23
	};

	m_Indices = indices;
	m_Vertices = vertices;
}
