//#define ENABLE_TESTS

#ifdef ENABLE_TESTS
#include "Tester.h"
#include "core/ecs/ECS.h"
int main()
{
	//Tester::runTests();
	Engine::Core::ECS::EntityComponentSystemManager ecs{};
	ecs.addComponent(ecs.createEntity(), Engine::Core::ECS::TransformComponent{});
	ecs.components.getComponent<Engine::Core::ECS::TransformComponent>(0).position = glm::vec3(1.0f, 2.0f, 3.0f);
	return 0;
}

#endif // !1
