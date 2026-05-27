#include "infra/app/Window.h"
#include <stdexcept>

using namespace Engine::Infra;
Window::Window(int width, int height, const char* windowTitle, GLFWmonitor* monitor, GLFWwindow* share)
	: width(width), height(height), windowTitle(windowTitle)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindow = glfwCreateWindow(width, height, windowTitle, monitor, share);

    if (!glfwWindow)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(glfwWindow);
}

Window::~Window()
{

	destroyWindow();
}

//We submit this in the application, since the applicaation is what owns the inputhandler.
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

