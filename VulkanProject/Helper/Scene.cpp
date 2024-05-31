#include "Helper/Scene.h"

#include "Mesh2D.h"
#include "Mesh3D.h"

void Scene::Init(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, VkDescriptorSetLayout descriptorSetLayout)
{
	for (const auto mesh : m_Meshes)
	{
		mesh->Init(physicalDevice, device, commandPool, graphicQueue, descriptorSetLayout);
	}
}

void Scene::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame , VkPipelineLayout pipelineLayout)
{
	for (const auto& mesh : m_Meshes)
	{
		mesh->Draw(commandBuffer, currentFrame, pipelineLayout);
	}
}

void Scene::Update(uint32_t currentImage, float deltaTime, VkExtent2D swapchainExtent, const Camera& camera)
{
	for (auto mesh : m_Meshes)
	{
		if(Mesh3D* mesh3D{dynamic_cast<Mesh3D*>(mesh)})
		{
			mesh3D->Update(currentImage,deltaTime,swapchainExtent, camera);
		}
	}
}

void Scene::Cleanup(VkDevice device) const
{
	for (auto mesh : m_Meshes)
	{
		mesh->Cleanup(device);
		delete mesh;
		mesh = nullptr;
	}
}

void Scene::AddMesh(Mesh* mesh)
{
	m_Meshes.emplace_back(mesh);
}

VkDescriptorPool Scene::GetDescriptorPool() const
{
	for (const auto mesh : m_Meshes)
	{
		if(Mesh3D* mesh3D{dynamic_cast<Mesh3D*>(mesh)})
		{
			return mesh3D->GetDescriptorPool();
		}
	}
	return VK_NULL_HANDLE;
}
