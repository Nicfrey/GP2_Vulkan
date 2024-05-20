#include "SphereMesh.h"

#include "MathHelpers.h"

SphereMesh::SphereMesh()
{
	constexpr int n_slices = 32;
	constexpr int n_stacks = 16;
    m_Vertices.clear();
    m_Vertices.clear();

    // add top vertex
    m_Vertices.push_back(Vertex3D{ {0, 1, 0}, {1, 1, 1}, {0, 0}, {0, 1, 0}, {0, 0, 0} });

    // generate vertices per stack / slice
    for (int i = 0; i < n_stacks - 1; i++)
    {
        double phi = MathHelper::PI * (i + 1) / n_stacks;
        float v = static_cast<float>(i + 1) / n_stacks; // Calculating the v texture coordinate
        for (int j = 0; j < n_slices; j++)
        {
            float u = static_cast<float>(j) / n_slices; // Calculating the u texture coordinate
            double theta = 2.0 * MathHelper::PI * j / n_slices;
            float x = std::sin(phi) * std::cos(theta);
            float y = std::cos(phi);
            float z = std::sin(phi) * std::sin(theta);
			glm::vec3 normal = glm::normalize(glm::vec3{ x, y, z });
            m_Vertices.push_back(Vertex3D{ {x, y, z}, {1, 1, 1}, {u, v}, normal, {0, 0, 0} });
        }
    }

    // add bottom vertex
    m_Vertices.push_back(Vertex3D{ {0, -1, 0}, {1, 1, 1}, {0, 0}, {0, -1, 0}, {0, 0, 0} });

    // add top / bottom triangles
    for (int i = 0; i < n_slices; ++i)
    {
        int i0 = i + 1;
        int i1 = (i + 1) % n_slices + 1;
        m_Indices.push_back(0);
        m_Indices.push_back(i0);
        m_Indices.push_back(i1);

        i0 = i + n_slices * (n_stacks - 2) + 1;
        i1 = (i + 1) % n_slices + n_slices * (n_stacks - 2) + 1;
        m_Indices.push_back(m_Vertices.size() - 1);
        m_Indices.push_back(i1);
        m_Indices.push_back(i0);
    }

    // add quads per stack / slice
    for (int j = 0; j < n_stacks - 2; j++)
    {
        int j0 = j * n_slices + 1;
        int j1 = (j + 1) * n_slices + 1;
        for (int i = 0; i < n_slices; i++)
        {
            int i0 = j0 + i;
            int i1 = j0 + (i + 1) % n_slices;
            int i2 = j1 + (i + 1) % n_slices;
            int i3 = j1 + i;
            m_Indices.push_back(i0);
            m_Indices.push_back(i1);
            m_Indices.push_back(i2);
            m_Indices.push_back(i2);
            m_Indices.push_back(i3);
            m_Indices.push_back(i0);
        }
    }
}
