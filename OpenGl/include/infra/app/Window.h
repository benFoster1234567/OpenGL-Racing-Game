#pragma once
#include <map>

#include <GLFW/glfw3.h>
#include "core/events/EventDispatcher.h"

class alignas(16) Window
{
private:
	size_t width = 720;
	size_t height = 480;
    
    const char* windowTitle;
    GLFWwindow* glfwWindow;
	//window, key, scancode, action, mods
	Engine::Core::EventDispatcher<int, int, int, int> keyPressedDispatcher;

public:
	Window(int width, int height, const char* windowTitle, GLFWmonitor* monitor, GLFWwindow* share);
	~Window();

	void pollEvents() const { glfwPollEvents(); }

    static void glfwKeyCallback(GLFWwindow* _glfwWindow, int _key, int _scancode, int _action, int _mods) {
        Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(_glfwWindow));
        if (window) {
            window->onKey(_key, _scancode, _action, _mods);
        }
    }
	
    void submitKeyCallback(std::function<void(int, int, int, int)> callback);
    void onKey(int key, int scancode, int action, int mods);
	void destroyWindow() { glfwDestroyWindow(glfwWindow); }
};

