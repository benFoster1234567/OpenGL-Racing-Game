#pragma once

#include "core/input/Keys.h"
#include <GLFW/glfw3.h>

namespace Engine::Infra
{
	Engine::Core::KeyCode glfwKeyToEngine(int key, int keyMod);
}




