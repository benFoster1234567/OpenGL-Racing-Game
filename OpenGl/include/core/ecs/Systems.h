#pragma once
#include "core/assets/MeshData.h"
#include "core/assets/ShaderData.h"
#include "core/events/EventDispatcher.h"
#include "core/ECS/Coordinator.h"
#include <core/input/KeyboardInput.h>
#include <core/input/MouseInputResource.h>
#include "core/ecs/System.h"


namespace Engine::Core::ECS
{

	struct RenderOutput
	{

		glm::mat4 view{1.0f};
		glm::mat4 projection{1.0f};
		glm::mat4 modelTransform{1.0f};
		ShaderData* shader{};
		MeshData* mesh{};
	};

	class RenderDispatcher : public System
	{
	public:
		static EventDispatcher<RenderOutput> sendRenderInfo; // connect renderer to this //
		virtual void update(Coordinator& coordinator, float aspect)
		{

			for (auto entity : entities)
			{
				glm::mat4 transformMat{};
				glm::mat4 cameraMat{};
				const auto& cameraComp = coordinator.getComponent<CameraComponent>(entity);
				const auto& transform = coordinator.getComponent<TransformComponent>(entity);
				const auto& meshData = coordinator.getComponent<MeshComponent>(entity);
				const auto& shaderData = coordinator.getComponent<ShaderComponent>(entity);
				//sendRenderInfo.invoke({ .mesh = meshData.meshData, .shader = shaderData.shaderData });
			}
		}
	};

	//this makes the camera follow the entity transform position.
	//requires camera, transform, mesh and shader components
	class RenderDispatcherOrbitalCamera : public RenderDispatcher
	{
		void update(Coordinator& coordinator, float aspect) override
		{
			for (auto entity : entities)
			{
				const auto& cameraComp = coordinator.getComponent<CameraComponent>(entity);
				const auto& transform = coordinator.getComponent<TransformComponent>(entity);
				const auto& meshData = coordinator.getComponent<MeshComponent>(entity);
				const auto& shaderData = coordinator.getComponent<ShaderComponent>(entity);

				glm::mat4 projectionMat = glm::perspective(cameraComp.fieldOfView,
					aspect, cameraComp.nearClipPlane, cameraComp.farClipPlane);

				glm::mat4 viewMat = glm::lookAt(transform.position + cameraComp.position, transform.position, { 0,1,0 });
				
				glm::mat4 transformMat = glm::translate(glm::mat4{}, transform.position)
					* glm::mat4_cast(transform.rotation)
					* glm::scale(glm::mat4{}, transform.scale);

				//invokes and sends to renderer
				sendRenderInfo.invoke({.view = viewMat
					, .projection = projectionMat
					, .modelTransform = transformMat
					, .shader = shaderData.shaderData
					, .mesh = meshData.meshData });
			}
		}
	};

	class KeyControlSystem : public System
	{
	public:
		void update(Coordinator& coordinator, const KeyboardBridge& inputHandler)
		{
		}
	};

	class MouseControlSystem : public System
	{
		void update(Coordinator& coordinator, Input::MouseInputResource& mouse)
		{

		}
	};


}