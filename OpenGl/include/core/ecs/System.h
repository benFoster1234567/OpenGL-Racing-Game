#pragma once

#include "ECS.h"

#include <set>

namespace Engine::Core::ECS
{
	class System
	{
	public:
		virtual ~System() = default;
		std::set<Entity> entities{};
	};


}