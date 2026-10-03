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
		std::unordered_map<std::type_index, Signature> m_signatures{};
		std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems{};

	public:
		template<std::derived_from<System> T>
		T* registerSystem()
		{
			std::type_index typeId = typeid(T);
			assert(!m_systems.contains(typeId) && "System already registered.");
			auto system = std::make_unique<T>();
			m_systems[typeId] = std::move(system);
			return static_cast<T*>(m_systems[typeId].get());
		}

		template <std::derived_from<System> T>
		T* getSystem()
		{
			std::type_index tid(typeid(T));
			assert(m_systems.contains(tid) && "System used before registered.");
			return static_cast<T*>(m_systems[tid].get());
		}

		template<class T>
		void setSignature(Signature signature)
		{
			std::type_index typeId = typeid(T);
			assert(m_systems.contains(typeId) && "System used before registered.");
			m_signatures.insert({ typeId, signature });
		}

		void entityDestroyed(Entity entity);
		void entitySignatureChanged(Entity entity, Signature entitySignature);

	};
}