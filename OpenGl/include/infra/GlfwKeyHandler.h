#pragma once
#include <GLFW/glfw3.h>
#include "Keys.h"
#include "InputHandler.h"

class GlfwKeyHandler
{
private:

	std::unordered_map<int, KeyCode> glfwKeyToKeyCode{};

public:
	GlfwKeyHandler();
	~GlfwKeyHandler() = default;
	GlfwKeyHandler(const GlfwKeyHandler&) = delete;
	GlfwKeyHandler(GlfwKeyHandler&&) = delete;

	KeyCode getInput(int glfwKey);
	
	void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

};

