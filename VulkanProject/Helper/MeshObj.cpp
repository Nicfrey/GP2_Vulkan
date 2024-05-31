#include "MeshObj.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <iostream>
#include <tiny_obj_loader.h>

#include "MathHelpers.h"

std::string MeshObj::m_ResourceFolder = "resources/";

MeshObj::MeshObj(const std::string& file)
{
	if (!LoadObj(file, m_Vertices, m_Indices))
	{
		throw std::runtime_error("Failed to load obj file");
	}
}

bool MeshObj::LoadObj(const std::string& filename, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices,
                      bool flipAxisAndWinding)
{
	std::ifstream file(m_ResourceFolder + filename);
	if (!file)
		return false;

	std::vector<glm::vec3> positions{};
	std::vector<glm::vec3> normals{};
	std::vector<glm::vec2> UVs{};

	vertices.clear();
	indices.clear();

	std::string sCommand;
	// start a while iteration ending when the end of file is reached (ios::eof)
	while (!file.eof())
	{
		//read the first word of the string, use the >> operator (istream::operator>>) 
		file >> sCommand;
		//use conditional statements to process the different commands	
		if (sCommand == "#")
		{
			// Ignore Comment
		}
		else if (sCommand == "v")
		{
			//Vertex
			float x, y, z;
			file >> x >> y >> z;

			positions.emplace_back(x, y, z);
		}
		else if (sCommand == "vt")
		{
			// Vertex TexCoord
			float u, v;
			file >> u >> v;
			UVs.emplace_back(u, 1 - v);
		}
		else if (sCommand == "vn")
		{
			// Vertex Normal
			float x, y, z;
			file >> x >> y >> z;

			normals.emplace_back(x, y, z);
		}
		else if (sCommand == "f")
		{
			//if a face is read:
			//construct the 3 vertices, add them to the vertex array
			//add three indices to the index array
			//add the material index as attibute to the attribute array
			//
			// Faces or triangles
			Vertex3D vertex{};
			size_t iPosition, iTexCoord, iNormal;

			uint32_t tempIndices[3];
			for (size_t iFace = 0; iFace < 3; iFace++)
			{
				// OBJ format uses 1-based arrays
				file >> iPosition;
				vertex.pos = positions[iPosition - 1];

				if ('/' == file.peek())//is next in buffer ==  '/' ?
				{
					file.ignore();//read and ignore one element ('/')

					if ('/' != file.peek())
					{
						// Optional texture coordinate
						file >> iTexCoord;
						vertex.textCoord = UVs[iTexCoord - 1];
					}

					if ('/' == file.peek())
					{
						file.ignore();

						// Optional vertex normal
						file >> iNormal;
						vertex.normal = normals[iNormal - 1];
					}
				}

				vertices.push_back(vertex);
				tempIndices[iFace] = uint32_t(vertices.size()) - 1;
			}

			indices.push_back(tempIndices[0]);
			if (flipAxisAndWinding)
			{
				indices.push_back(tempIndices[2]);
				indices.push_back(tempIndices[1]);
			}
			else
			{
				indices.push_back(tempIndices[1]);
				indices.push_back(tempIndices[2]);
			}
		}
		//read till end of line and ignore all remaining chars
		file.ignore(1000, '\n');
	}

	//Cheap Tangent Calculations
	for (uint32_t i = 0; i < indices.size(); i += 3)
	{
		uint32_t index0 = indices[i];
		uint32_t index1 = indices[size_t(i) + 1];
		uint32_t index2 = indices[size_t(i) + 2];

		const glm::vec3& p0 = vertices[index0].pos;
		const glm::vec3& p1 = vertices[index1].pos;
		const glm::vec3& p2 = vertices[index2].pos;
		const glm::vec2& uv0 = vertices[index0].textCoord;
		const glm::vec2& uv1 = vertices[index1].textCoord;
		const glm::vec2& uv2 = vertices[index2].textCoord;

		const glm::vec3 edge0 = p1 - p0;
		const glm::vec3 edge1 = p2 - p0;
		const glm::vec2 diffX = glm::vec2(uv1.x - uv0.x, uv2.x - uv0.x);
		const glm::vec2 diffY = glm::vec2(uv1.y - uv0.y, uv2.y - uv0.y);
		float r = 1.f / (diffX.x * diffY.y - diffX.y * diffY.x);

		glm::vec3 tangent = (edge0 * diffY.y - edge1 * diffY.x) * r;
		vertices[index0].tangent += tangent;
		vertices[index1].tangent += tangent;
		vertices[index2].tangent += tangent;
	}

	//Fix the tangents per vertex now because we accumulated
	for (auto& v : vertices)
	{
		glm::vec3 reject = (v.tangent - v.normal * (glm::dot(v.tangent, v.normal) / glm::dot(v.normal, v.normal)));
		v.tangent = reject;

		if (flipAxisAndWinding)
		{
			v.pos.z *= -1.f;
			v.normal.z *= -1.f;
			v.tangent.z *= -1.f;
		}
	}
	return true;
}

void MeshObj::CalculateTangent(std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices)
{
	for (size_t i = 0; i < indices.size(); i += 3) {
		Vertex3D& v0 = vertices[indices[i]];
		Vertex3D& v1 = vertices[indices[i + 1]];
		Vertex3D& v2 = vertices[indices[i + 2]];

		const glm::vec3 edge1 = v1.pos - v0.pos;
		const glm::vec3 edge2 = v2.pos - v0.pos;

		const glm::vec2 deltaUV1 = v1.textCoord - v0.textCoord;
		const glm::vec2 deltaUV2 = v2.textCoord - v0.textCoord;

		const float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		v0.tangent += tangent;
		v1.tangent += tangent;
		v2.tangent += tangent;
	}

	for (auto& vertex : vertices) {
		vertex.tangent = glm::normalize(vertex.tangent);
	}
}

void MeshObj::GenerateNormals(tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes)
{
	attrib.normals.resize(attrib.vertices.size(), 0.0f);

	for (const auto& shape : shapes) {
		for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
			tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
			tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
			tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

			glm::vec3 v0 = glm::vec3(attrib.vertices[3 * idx0.vertex_index + 0],
				attrib.vertices[3 * idx0.vertex_index + 1],
				attrib.vertices[3 * idx0.vertex_index + 2]);
			glm::vec3 v1 = glm::vec3(attrib.vertices[3 * idx1.vertex_index + 0],
				attrib.vertices[3 * idx1.vertex_index + 1],
				attrib.vertices[3 * idx1.vertex_index + 2]);
			glm::vec3 v2 = glm::vec3(attrib.vertices[3 * idx2.vertex_index + 0],
				attrib.vertices[3 * idx2.vertex_index + 1],
				attrib.vertices[3 * idx2.vertex_index + 2]);

			glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

			for (int i = 0; i < 3; ++i) {
				attrib.normals[3 * idx0.vertex_index + i] += normal[i];
				attrib.normals[3 * idx1.vertex_index + i] += normal[i];
				attrib.normals[3 * idx2.vertex_index + i] += normal[i];
			}
		}
	}

	for (size_t i = 0; i < attrib.normals.size(); i += 3) {
		glm::vec3 normal = glm::vec3(attrib.normals[i + 0], attrib.normals[i + 1], attrib.normals[i + 2]);
		normal = glm::normalize(normal);
		attrib.normals[i + 0] = normal.x;
		attrib.normals[i + 1] = normal.y;
		attrib.normals[i + 2] = normal.z;
	}
}

void MeshObj::Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue,
                   VkDescriptorSetLayout descriptorLayout)
{
	Mesh3D::Init(physicalDevice, device, commandPool, graphicQueue, descriptorLayout);
}

void MeshObj::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame, VkPipelineLayout pipelineLayout) const
{
	Mesh3D::Draw(commandBuffer, currentFrame, pipelineLayout);
}

void MeshObj::Cleanup(VkDevice device) const
{
	Mesh3D::Cleanup(device);
}
