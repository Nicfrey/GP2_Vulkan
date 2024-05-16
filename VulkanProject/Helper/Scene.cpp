#include "Helper/Scene.h"

void Scene::Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,VkQueue queue)
{
	for (auto& mesh : m_Meshes)
	{
		mesh.Init(physicalDevice,device,commandPool,queue);
	}
}

void Scene::Draw(VkCommandBuffer commandBuffer) const
{
	for (auto& mesh : m_Meshes)
	{
		mesh.Draw(commandBuffer);
	}
}

void Scene::Cleanup(VkDevice device) const
{
	for (auto& mesh : m_Meshes)
	{
		mesh.Cleanup(device);
	}
}

void Scene::AddMesh(const Mesh& mesh)
{
	m_Meshes.emplace_back(mesh);
}
