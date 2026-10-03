#pragma once
#include "ComponentRegistry.h"
#include "EntityRegistry.h"
#include "SystemRegistry.h"
namespace Engine::Core::ECS
{
	class Coordinator
	{
	private:
		std::unique_ptr<ComponentRegistry> m_componentRegistry{};
		std::unique_ptr<EntityRegistry> m_entityRegistry{};
		std::unique_ptr<SystemRegistry> m_systemRegistry{};

	public:
		Coordinator() :
			m_componentRegistry{ std::make_unique<ComponentRegistry>() },
			m_entityRegistry{ std::make_unique<EntityRegistry>() },
			m_systemRegistry{ std::make_unique<SystemRegistry>() }
		{
		}

		Coordinator(const Coordinator&) = delete;
		Coordinator& operator=(const Coordinator&) = delete;

		Coordinator(Coordinator&&) noexcept = default;
		Coordinator& operator=(Coordinator&&) noexcept = default;

		Entity createEntity()
		{
			return m_entityRegistry->createEntity();
		}

		void destroyEntity(Entity entity)
		{
			m_entityRegistry->destroyEntity(entity);
			m_componentRegistry->entityDestroyed(entity);
			m_systemRegistry->entityDestroyed(entity);
		}

		// Component methods
		template<class T>
		void registerComponent()
		{
			m_componentRegistry->registerComponent<T>();
		}

		template<class T>
		void addComponent(Entity entity, T component)
		{
			m_componentRegistry->addComponent<T>(entity, component);

			auto signature = m_entityRegistry->getSignature(entity);
			signature.set(m_componentRegistry->getComponentType<T>(), true);
			m_entityRegistry->setSignature(entity, signature);

			m_systemRegistry->entitySignatureChanged(entity, signature);
		}

		template<class T>
		void removeComponent(Entity entity)
		{
			m_componentRegistry->removeComponent<T>(entity);

			auto signature = m_entityRegistry->getSignature(entity);
			signature.set(m_componentRegistry->getComponentType<T>(), false);
			m_entityRegistry->setSignature(entity, signature);

			m_systemRegistry->entitySignatureChanged(entity, signature);
		}

		template<class T>
		T& getComponent(Entity entity)
		{
			return m_componentRegistry->getComponent<T>(entity);
		}

		template<class T>
		ComponentType getComponentType()
		{
			return m_componentRegistry->getComponentType<T>();
		}

		// System methods
		template<typename T>
		T* registerSystem()
		{
			return m_systemRegistry->registerSystem<T>();
		}

		template<typename T>
		T* getSystem()
		{
			return m_systemRegistry->getSystem<T>();
		}

		template<typename T>
		void setSystemSignature(Signature signature)
		{
			m_systemRegistry->template setSignature<T>(signature);
		}
	};
}