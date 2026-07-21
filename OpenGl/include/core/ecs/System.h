#pragma once

#include "ECS.h"

#include <set>

namespace Engine::Core::ECS
{
	class System
	{
	public:
		std::set<Entity> entities{};
	};


}