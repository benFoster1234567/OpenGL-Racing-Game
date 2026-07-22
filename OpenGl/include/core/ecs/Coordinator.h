#pragma once
#include "ComponentRegistry.h"
#include "EntityRegistry.h"
#include "SystemRegistry.h"
namespace Engine::Core::ECS
{
	class Coordinator
	{
	private:
		std::unique_ptr<ComponentRegistry> componentRegistry{};
		std::unique_ptr<EntityRegistry> entityRegistry{};
		std::unique_ptr<SystemRegistry> systemRegistry{};

	public:
		Coordinator() :
			componentRegistry{ std::make_unique<ComponentRegistry>() },
			entityRegistry{ std::make_unique<EntityRegistry>() },
			systemRegistry{ std::make_unique<SystemRegistry>() }
		{
		}

		Coordinator(const Coordinator&) = delete;
		Coordinator& operator=(const Coordinator&) = delete;

		Coordinator(Coordinator&&) noexcept = default;
		Coordinator& operator=(Coordinator&&) noexcept = default;

		Entity createEntity()
		{
			return entityRegistry->createEntity();
		}

		void destroyEntity(Entity entity)
		{
			entityRegistry->destroyEntity(entity);
			componentRegistry->entityDestroyed(entity);
			systemRegistry->entityDestroyed(entity);
		}

		// Component methods
		template<class T>
		void registerComponent()
		{
			componentRegistry->registerComponent<T>();
		}

		template<class T>
		void addComponent(Entity entity, T component)
		{
			componentRegistry->addComponent<T>(entity, component); 

			auto signature = entityRegistry->getSignature(entity); 
			signature.set(componentRegistry->getComponentType<T>(), true);
			entityRegistry->setSignature(entity, signature);

			systemRegistry->entitySignatureChanged(entity, signature); 
		}

		template<class T>
		void removeComponent(Entity entity)
		{
			componentRegistry->removeComponent<T>(entity);

			auto signature = entityRegistry->getSignature(entity);
			signature.set(componentRegistry->getComponentType<T>(), false);
			entityRegistry->setSignature(entity, signature);

			systemRegistry->entitySignatureChanged(entity, signature); 
		}

		template<class T>
		T& getComponent(Entity entity)
		{
			return componentRegistry->getComponent<T>(entity); 
		}

		template<class T>
		ComponentType getComponentType()
		{
			return componentRegistry->getComponentType<T>();
		}

		// System methods
		template<typename T>
		T* registerSystem() 
		{
			return systemRegistry->registerSystem<T>(); 
		}

		template<typename T>
		void setSystemSignature(Signature signature)
		{
			systemRegistry->template setSignature<T>(signature); 
		}
	};
}