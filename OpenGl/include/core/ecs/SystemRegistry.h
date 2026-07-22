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
		template<std::derived_from<System> T>
		T* registerSystem()
		{
			std::type_index typeId = typeid(T);
			assert(!systems.contains(typeId) && "System already registered.");
			auto system = std::make_unique<T>();
			systems[typeId] = std::move(system);
			return static_cast<T*>(systems[typeId].get());
		}

		template<class T>
		void setSignature(Signature signature)
		{
			std::type_index typeId = typeid(T);
			assert(systems.contains(typeId) && "System used before registered.");
			signatures.insert({ typeId, signature });
		}

		void entityDestroyed(Entity entity);
		void entitySignatureChanged(Entity entity, Signature entitySignature);

	};
}