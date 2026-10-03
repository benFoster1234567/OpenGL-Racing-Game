#include "RenderDispatcherSystems.h"

#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../coordinator/Coordinator.h"
#include "../components/CameraComponents.h"
#include "../components/TransformComponent.h"
#include "../components/MeshComponent.h"
#include "../components/ShaderComponent.h"
#include "../components/MaterialDataComponent.h"

namespace Engine::Core::ECS
{
	void RenderDispatcherOrbitalCamera::update(Coordinator& coordinator, float aspect)
	{
		for (auto entity : m_entities)
		{
			auto& cameraComp = coordinator.getComponent<CameraComponent>(entity);
			const auto& transform = coordinator.getComponent<TransformComponent>(entity);
			const auto& meshData = coordinator.getComponent<MeshComponent>(entity);
			const auto& shaderData = coordinator.getComponent<ShaderComponent>(entity);
			const auto& orbitalCam = coordinator.getComponent<OrbitalCameraComponent>(entity);
			const auto& material = coordinator.getComponent<MaterialDataComponent>(entity);

			if (material.material == nullptr)
			{
				throw std::runtime_error("MaterialDataComponent is null for entity " + std::to_string(entity));
			}

			glm::mat4 projectionMat = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 1000.0f);
			cameraComp.projectionMat = projectionMat;
			glm::mat4 viewMat = glm::lookAt(transform.position + cameraComp.position, transform.position, { 0.0f, 1.0f, 0.0f });
			cameraComp.viewMat = viewMat;
			glm::mat4 transformMat = glm::translate(glm::mat4{ 1.0f }, transform.position)
				* glm::mat4_cast(transform.rotation)
				* glm::scale(glm::mat4{ 1.0f }, transform.scale);

			sendRenderInfo.invoke({ .view = viewMat
				, .projection = projectionMat
				, .modelTransform = transformMat
				, .uvScale = meshData.uvScale
				, .shader = shaderData.shaderId
				, .mesh = meshData.meshId
				, .material = material.material });
		}
	}

	void RenderDispatcherExternalCamera::update(Coordinator& coordinator, float aspect)
	{
		for (auto entity : m_entities)
		{
			const auto& extCameraComp = coordinator.getComponent<ExternalCameraComponent>(entity);
			const auto& cameraComp = coordinator.getComponent<CameraComponent>(extCameraComp.entityWithCamera);
			const auto& transform = coordinator.getComponent<TransformComponent>(entity);
			const auto& meshData = coordinator.getComponent<MeshComponent>(entity);
			const auto& shaderData = coordinator.getComponent<ShaderComponent>(entity);
			const auto& material = coordinator.getComponent<MaterialDataComponent>(entity);

			if (material.material == nullptr)
			{
				throw std::runtime_error("MaterialDataComponent is null for entity " + std::to_string(entity));
			}

			glm::mat4 projectionMat = cameraComp.projectionMat;
			glm::mat4 viewMat = cameraComp.viewMat;

			glm::mat4 transformMat = glm::translate(glm::mat4{ 1.0f }, transform.position)
				* glm::mat4_cast(transform.rotation)
				* glm::scale(glm::mat4{ 1.0f }, transform.scale);

			sendRenderInfo.invoke({ .view = viewMat
				, .projection = projectionMat
				, .modelTransform = transformMat
				, .uvScale = meshData.uvScale
				, .shader = shaderData.shaderId
				, .mesh = meshData.meshId
				, .material = material.material });
		}
	}
}