#pragma once

#include "ECS.h"
#include <cassert>
#include <unordered_map>
#include <stdexcept>

namespace Engine::Core::ECS
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default; // Added default implementation
		virtual void entityDestroyed(Entity entity) = 0;
	};

	template <std::derived_from<ComponentBase> T>
	class ComponentArray : public IComponentArray
	{
	private:
		size_t m_size{ 0 };
		std::array<T, MAX_COMPONENTS> m_componentArray{};
		std::unordered_map<Entity, size_t> m_entityToIndexMap{};
		std::unordered_map<size_t, Entity> m_indexToEntityMap{};

	public:
		void addData(Entity entity, T component)
		{
			if (m_size >= MAX_COMPONENTS)
			{
				throw std::runtime_error("Maximum number of components reached.");
			}

			size_t newIndex = m_size;
			m_entityToIndexMap[entity] = newIndex;
			m_indexToEntityMap[newIndex] = entity;
			m_componentArray[newIndex] = component;
			++m_size;
		}

		void removeData(Entity entity)
		{
			assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Removing non-existent component.");

			size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
			size_t indexOfLastElement = m_size - 1;
			m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastElement];

			Entity entityOfLastElement = m_indexToEntityMap[indexOfLastElement];
			m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			m_entityToIndexMap.erase(entity);
			m_indexToEntityMap.erase(indexOfLastElement);
			--m_size;
		}

		T& getData(Entity entity)
		{
			assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Retrieving non-existent component.");
			return m_componentArray[m_entityToIndexMap[entity]];
		}

		void entityDestroyed(Entity entity) override
		{
			if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end())
			{
				removeData(entity);
			}
		}
	};
}