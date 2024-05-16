#pragma once
#include <vector>

#include "Mesh.h"

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	Scene(const Scene& other) = delete;
	Scene(Scene&& other) noexcept = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) noexcept = delete;

	void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue queue);
	void Draw(VkCommandBuffer commandBuffer) const;
	void Cleanup(VkDevice device) const;
	void AddMesh(const Mesh& mesh);
private:
	std::vector<Mesh> m_Meshes{};
};
