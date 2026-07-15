#ifndef ECS_H 
#define ECS_H

#include <array>          
#include <unordered_map>  
#include <functional>     
#include <memory>
#include <string>
#include <glm/mat4x4.hpp>
#include <cstdint>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float4x4.hpp>


#include "core/assets/MeshData.h"
#include "core/assets/ShaderData.h"
#include "core/assets/MaterialData.h"
#include <typeindex>
#include <bitset>
#include <glm/gtc/quaternion.hpp>



namespace Engine::Core::ECS
{

	using Entity = std::uint16_t;
	const size_t MAX_ENTITIES = 1000;
	
	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 32;

	using System = size_t;


	struct ComponentBase
	{
		virtual ~ComponentBase();
	};

	struct CameraComponent : public ComponentBase
	{
		glm::mat4 projection{};
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		void rotate(float degrees, glm::vec3 axis);
	};

	struct TransformComponent : public ComponentBase
	{
		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f }; // Quaternion representation
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

		glm::mat4 getTransformMatrix() const
		{
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
			glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
			return translationMatrix * rotationMatrix * scaleMatrix;
		}

	};

	struct MeshComponent : public ComponentBase
	{
		MeshComponent() = default;
		MeshComponent(Engine::Core::MeshData* mesh) : meshData(mesh) {}
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(MeshComponent&&) = default;
		MeshComponent& operator=(const MeshComponent&) = default;
		MeshComponent& operator=(MeshComponent&&) = default;
		~MeshComponent() = default;

		Engine::Core::MeshData* meshData{ nullptr };
	};

	struct ShaderComponent : public ComponentBase
	{
		ShaderComponent() = default;
		ShaderComponent(Engine::Core::ShaderData* shader) : shaderData(shader) {}
		ShaderComponent(const ShaderComponent&) = default;
		ShaderComponent(ShaderComponent&&) = default;
		ShaderComponent& operator=(const ShaderComponent&) = default;
		ShaderComponent& operator=(ShaderComponent&&) = default;
		~ShaderComponent() = default;

		Engine::Core::ShaderData* shaderData{ nullptr };
	};

	struct MaterialComponent : public ComponentBase
	{
		MaterialComponent() = default;
		MaterialComponent(ShaderData* _shader, MaterialData* _material) : shader{ _shader }, material{ _material } {}

		~MaterialComponent() = default;

		ShaderData* shader{nullptr};
		MaterialData* material{nullptr};
	};

	class IComponentArray
	{
	public:
		virtual ~IComponentArray();
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
			throw std::runtime_error("removeData not implemented yet.");
		}

		T& getData(Entity entity)
		{
			return componentArray[entityToIndexMap[entity]];
		}

	};

	class ComponentRegistry
	{

	private:

		std::unordered_map <std::type_index, std::unique_ptr<IComponentArray>> componentArrays{};

	public:

		template <std::derived_from<ComponentBase> T>
		void registerComponent(Entity entity, T component)
		{
			//check to see if this works with an object that contains a pointer to a mesh or shader. 
			// I think it should be fine, but we need to make sure that the component is not being 
			// copied and that the pointer is still valid when we access it later.

			// each entity has its own copy of the component.

			if (!componentArrays.contains(std::type_index(typeid(T))))
			{
				componentArrays[std::type_index(typeid(T))] = std::make_unique<ComponentArray<T>>();
			}

			auto componentArray = static_cast<ComponentArray<T>*>(componentArrays[std::type_index(typeid(T))].get());
			componentArray->addData(entity, component);
		}

		template <std::derived_from<ComponentBase> T>
		T& getComponent(Entity entity)
		{
			if (!componentArrays.contains(std::type_index(typeid(T))))
			{
				throw std::runtime_error("Component not registered.");
			}
			auto componentArray = static_cast<ComponentArray<T>*>(componentArrays[std::type_index(typeid(T))].get());
			return componentArray->getData(entity);
		}

	};

	class SystemRegistry
	{

	public:
		using SystemFunction = std::function<void(Entity, ComponentRegistry&)>;
		System registerSystem(SystemFunction systemFunc); //later we can add in a signature that keeps track of which components are required for the system to run. For now, we will just assume that the system can run on any entity.
	
	private:
		System nextSystem{ 0 };
		std::unordered_map<System, SystemFunction> systemMap{};
		friend class EntityComponentSystemManager;
	};

	class EntityComponentSystemManager
	{
	private:
		//using SystemTag = std::bitset<512>;
		Entity nextEntityId{ 0 };
		std::unordered_map<Entity, std::vector<System>> entitySystemMap{};
		std::vector<Entity> entities{};
	public:
		
		ComponentRegistry components{};
		SystemRegistry systems{};

		Entity createEntity();

		std::vector<Entity> getEntities() const { return entities; }

		void addComponent(Entity entity, std::derived_from<ComponentBase> auto component)
		{
			components.registerComponent(entity, component);
		}



		void attachSystem(Entity entity, System system);

		void updateSystems();
	};
}


#endif