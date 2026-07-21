#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "System.h"

namespace Engine::Core::ECS
{
	class SystemRegistry
	{
	private:
		std::unordered_map<std::type_index, Signature> signatures{};
		std::unordered_map<std::type_index, std::unique_ptr<System>> systems{};
	
	public:
		template<class T>
		T* registerSystem()
		{
			auto typeId = std::type_index(typeid(T));
			assert(!systems.contains(typeId) && "System already registered.");
			auto system = std::make_unique<T>();
			systems[typeId] = std::move(system);
			return systems[typeId].get();
		}

		template<class T>
		void setSignature(Signature signature)
		{
			auto typeId = std::type_index(typeid(T));
			assert(systems.contains(typeId) && "System sig already set.");
			signatures.insert({ typeId, signature });
		}

		void entityDestroyed(Entity entity);
		void entitySignatureChanged(Entity entity, Signature entitySignature);

	};
}