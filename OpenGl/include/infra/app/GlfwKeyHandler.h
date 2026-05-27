#pragma once
#include "core/input/Keys.h"
#include "core/input/InputHandler.h"
#include <GLFW/glfw3.h>
#include <unordered_map>

namespace Engine::Infra
{

	class GlfwKeyHandler
	{
	private:

		std::unordered_map<int, Core::KeyCode> glfwKeyToKeyCode{};

	public:
		GlfwKeyHandler();
		~GlfwKeyHandler() = default;
		GlfwKeyHandler(const GlfwKeyHandler&) = delete;
		GlfwKeyHandler(GlfwKeyHandler&&) = delete;

		Core::KeyCode getInput(int glfwKey);
	};

}