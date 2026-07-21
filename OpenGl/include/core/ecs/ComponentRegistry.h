#pragma once
#include "Component.h"
#include <typeindex>
#include <unordered_set>
#include "ComponentArray.h"
namespace Engine::Core::ECS
{
	class ComponentRegistry
	{
	private:
		std::unordered_map <std::type_index, std::unique_ptr<IComponentArray>> componentArrays{};
		std::unordered_map <std::type_index, ComponentType> componentTypes{};
		ComponentType nextComponentType{};

		template<typename T>
		ComponentArray<T>* getComponentArray()
		{
			auto typeIdx = std::type_index(typeid(T));
			assert(componentTypes.find(typeIdx) != componentTypes.end() && "Component not registered before use.");
			return static_cast<ComponentArray<T>*>(componentArrays[typeIdx].get()); 
		}

	public:
		template <std::derived_from<ComponentBase> T>
		void registerComponent()
		{
			auto typeIdx = std::type_index(typeid(T));
			assert(componentTypes.find(typeIdx) == componentTypes.end() && "Registering component type more than once.");

			componentTypes[typeIdx] = nextComponentType;
			componentArrays[typeIdx] = std::make_unique<ComponentArray<T>>();
			nextComponentType++;
		}

		template <class T>
		ComponentType getComponentType()
		{
			std::type_index typeIdx(typeid(T));
			assert(componentTypes.contains(typeIdx) && "Component not registered before use");
			return componentTypes[typeIdx];
		}

		template <class T>
		void addComponent(Entity entity, T component)
		{
			getComponentArray<T>()->addData(entity, component);
		}

		template <std::derived_from<ComponentBase> T>
		T& getComponent(Entity entity)
		{
			return getComponentArray<T>()->getData(entity);
		}

		template <std::derived_from<ComponentBase> T>
		void removeComponent(Entity entity)
		{
			getComponentArray<T>()->removeData(entity);
		}

		void entityDestroyed(Entity entity);
	};
}