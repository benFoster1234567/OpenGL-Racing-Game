#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "core/events/EventDispatcher.h"
#include <functional>
#include <string>
#include <GL/gl.h>
#include <bitset>

namespace Engine::Infra
{


    class alignas(16) Window
    {
    private:


        // for fullscreen
        struct 
        {
            int x, y, w, h;
        } savedWindowState{ 0,0,0,0 };

        void saveWindowState(int x, int y, int w, int h);
        void saveWindowState();
        //

        int width = 0;
        int height = 0;

        bool isFullscreen{true};

        const char* windowTitle;
        Engine::Core::EventDispatcher<int, int, int, int> keyPressedDispatcher;
		friend class DebugConsoleUi;
    public:

        Window(const std::string& _windowTitle = "OpenGL Window", bool _isFullscreen = true, int _width = 1600, int _height = 900, int posX = 10, int posY = 24);

		Window(const Window&) = default;
		Window(Window&&) = default;
        ~Window();
        GLFWwindow* glfwWindow;
        bool closeApplication{ false };

        void submitKeyCallback(std::function<void(int, int, int, int)> callback);

        void pollEvents() const { glfwPollEvents(); }

        std::bitset<512> pollEventsGetKeys();


        static void glfwKeyCallback(GLFWwindow* _glfwWindow, int _key, int _scancode, int _action, int _mods) 
        {
            Window* window = static_cast<Window*>(glfwGetWindowUserPointer(_glfwWindow));
            if (window) 
            {
                window->onKey(_key, _scancode, _action, _mods);
            }
        }

        void setFullscreen();

        void setWindowed();

        void setWindowSize(int w, int h);

        int getWidth() { return this->width; };
        int getHeight() { return this->height; };

		void updateViewport()
		{
			glfwGetFramebufferSize(glfwWindow, &width, &height);
			glViewport(0, 0, width, height);
		}

		void swapBuffers() const { glfwSwapBuffers(glfwWindow); }
        bool shouldClose() const { return closeApplication || glfwWindowShouldClose(glfwWindow); }
        void onKey(int key, int scancode, int action, int mods);

        void destroyWindow() 
        { 
            if (glfwWindow) {
                glfwDestroyWindow(glfwWindow);
                glfwWindow = nullptr;
            }
            glfwTerminate();
        }

		void terminateGlfw()
		{
			glfwTerminate();
		}
    };

}

