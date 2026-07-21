#ifndef ECS_H 
#define ECS_H
#include <cstdint>
#include <bitset>

namespace Engine::Core::ECS
{

	using Entity = std::uint16_t;
	const size_t MAX_ENTITIES = 1000;
	
	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;
}


#endif