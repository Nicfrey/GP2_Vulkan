#include "SphereMesh.h"

#include "MathHelpers.h"
#include "MeshObj.h"


SphereMesh::SphereMesh(const glm::vec3& center, float radius, int nbStacks, int nbSlices)
{
    SetPosition(center);
    CreateSphere(center, radius, nbStacks, nbSlices);
    MeshObj::CalculateTangent(m_Vertices,m_Indices);
}

void SphereMesh::CreateSphere(const glm::vec3& center, float radius, int nbStacks, int nbSlices)
{
    m_Vertices.clear();
    m_Vertices.clear();

    // add top vertex
    m_Vertices.push_back(Vertex3D{ glm::vec3{glm::vec3{0, radius, 0}}, {1, 1, 1}, {0.f, 0.f}, {0, 1, 0}, {0, 0, 0} });

    // generate vertices per stack / slice
    for (int i = 0; i < nbStacks - 1; i++)
    {
        float phi = MathHelper::PI * static_cast<float>(i + 1) / static_cast<float>(nbStacks);
        float v = static_cast<float>(i + 1) / static_cast<float>(nbStacks); // Calculating the v texture coordinate
        for (int j = 0; j < nbSlices; j++)
        {
            float u = static_cast<float>(j) / static_cast<float>(nbSlices); // Calculating the u texture coordinate
            const float theta = 2.0f * MathHelper::PI * static_cast<float>(j) / static_cast<float>(nbSlices);
            const float x = radius * std::sinf(phi) * std::cosf(theta);
            const float y = radius * std::cosf(phi);
            const float z = radius * std::sinf(phi) * std::sinf(theta);

            glm::vec3 pos = glm::vec3(glm::vec3{ x, y, z });
            glm::vec3 normal = glm::normalize(pos);
            m_Vertices.push_back(Vertex3D{ pos, {1, 1, 1}, {u, v}, normal, {0, 0, 0} });
        }
    }

    // add bottom vertex
    m_Vertices.push_back(Vertex3D{ glm::vec3{glm::vec3{0, -radius, 0}}, {1, 1, 1}, {1.f, 1.f}, {0, -1, 0}, {0, 0, 0} });

    // add top / bottom triangles
    for (int i = 0; i < nbSlices; ++i) {
        int i0 = i + 1;
        int i1 = (i + 1) % nbSlices + 1;
        m_Indices.push_back(0); // Top vertex index
        m_Indices.push_back(i1);
        m_Indices.push_back(i0);
    }

    // Add bottom triangles
    for (int i = 0; i < nbSlices; ++i) {
        int i0 = i + nbSlices * (nbStacks - 2) + 1;
        int i1 = (i + 1) % nbSlices + nbSlices * (nbStacks - 2) + 1;
        m_Indices.push_back(static_cast<uint32_t>(m_Vertices.size()) - 1); // Bottom vertex index
        m_Indices.push_back(i0);
        m_Indices.push_back(i1);
    }

    // add quads per stack / slice
    for (int j = 0; j < nbStacks - 2; j++)
    {
        int j0 = j * nbSlices + 1;
        int j1 = (j + 1) * nbSlices + 1;
        for (int i = 0; i < nbSlices; i++)
        {
            int i0 = j0 + i;
            int i1 = j0 + (i + 1) % nbSlices;
            int i2 = j1 + (i + 1) % nbSlices;
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
