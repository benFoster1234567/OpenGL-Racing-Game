#pragma once
#include "Component.h"
#include "ECS.h"
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
		size_t size{ 0 };
		std::array<T, MAX_COMPONENTS> componentArray{};
		std::unordered_map<Entity, size_t> entityToIndexMap{};
		std::unordered_map<size_t, Entity> indexToEntityMap{};

	public:
		void addData(Entity entity, T component)
		{
			if (size >= MAX_COMPONENTS)
			{
				throw std::runtime_error("Maximum number of components reached.");
			}

			size_t newIndex = size;
			entityToIndexMap[entity] = newIndex;
			indexToEntityMap[newIndex] = entity;
			componentArray[newIndex] = component;
			++size;
		}

		void removeData(Entity entity)
		{
			assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Removing non-existent component.");

			size_t indexOfRemovedEntity = entityToIndexMap[entity];
			size_t indexOfLastElement = size - 1;
			componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];

			Entity entityOfLastElement = indexToEntityMap[indexOfLastElement];
			entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			entityToIndexMap.erase(entity);
			indexToEntityMap.erase(indexOfLastElement);
			--size;
		}

		T& getData(Entity entity)
		{
			assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Retrieving non-existent component.");
			return componentArray[entityToIndexMap[entity]];
		}

		void entityDestroyed(Entity entity) override
		{
			if (entityToIndexMap.find(entity) != entityToIndexMap.end())
			{
				removeData(entity);
			}
		}
	};
}