#pragma once
#include <map>

#include <GLFW/glfw3.h>
#include "core/events/EventDispatcher.h"

namespace Engine::Infra
{
    class alignas(16) Window
    {
    private:
        size_t width = 720;
        size_t height = 480;

        const char* windowTitle;
        bool closeApplication{ false };
        Engine::Core::EventDispatcher<int, int, int, int> keyPressedDispatcher;
		friend class DebugConsoleUi;
    public:
        Window(int width, int height, const char* windowTitle, GLFWmonitor* monitor, GLFWwindow* share);
        ~Window();
        GLFWwindow* glfwWindow;

        void submitKeyCallback(std::function<void(int, int, int, int)> callback);

        void pollEvents() const { glfwPollEvents(); }

        static void glfwKeyCallback(GLFWwindow* _glfwWindow, int _key, int _scancode, int _action, int _mods) 
        {
            Window* window = static_cast<Window*>(glfwGetWindowUserPointer(_glfwWindow));
            if (window) 
            {
                window->onKey(_key, _scancode, _action, _mods);
            }
        }


		void updateViewport() const
		{
			int w, h;
			glfwGetFramebufferSize(glfwWindow, &w, &h);
			glViewport(0, 0, w, h);
		}
		void swapBuffers() const { glfwSwapBuffers(glfwWindow); }
        bool shouldClose() const { return closeApplication || glfwWindowShouldClose(glfwWindow); }
        void onKey(int key, int scancode, int action, int mods);
        void destroyWindow() { glfwDestroyWindow(glfwWindow); }
    };

}

