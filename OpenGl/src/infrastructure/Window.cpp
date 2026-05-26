#include "infra/app/Window.h"

Window::Window(int width, int height, const char* windowTitle, GLFWmonitor* monitor, GLFWwindow* share)
	: width(width), height(height), windowTitle(windowTitle)
{
	glfwWindow = glfwCreateWindow(width, height, windowTitle, monitor, share);
}

Window::~Window()
{

	destroyWindow();
}

void Window::submitKeyCallback(std::function<void(int, int, int, int)> callback)
{
	keyPressedDispatcher.subscribe(callback);
	glfwSetKeyCallback(glfwWindow, glfwKeyCallback);
	glfwSetWindowUserPointer(glfwWindow, this);
}

void Window::onKey(int key, int scancode, int action, int mods)
{
	keyPressedDispatcher.invoke(key, scancode, action, mods);
}

