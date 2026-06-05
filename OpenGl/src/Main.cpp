//#define ENABLE_TESTS
#ifndef ENABLE_TESTS

#include <GL/glew.h>
#include "infra/app/Window.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "infra/engine/DebugConsoleUI.h"
int main()
{
    if (!glfwInit()) { return -1; }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	int w = mode->width, h = mode->height;

    Engine::Infra::Window myWindow(w, h, "Test Window", monitor, nullptr);
	Engine::Infra::DebugConsoleUi debugConsoleUi(myWindow, "#version 430");
	
    std::function <std::string()> exitFunc = [&myWindow]() 
    {
        myWindow.closeApplication = true;
        return "Exiting..."; 
    };

    debugConsoleUi.registerCommand<>("exit", exitFunc);

    while (!myWindow.shouldClose())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);      
        myWindow.updateViewport();
        debugConsoleUi.prepareFrame();     
        debugConsoleUi.render();           
        myWindow.swapBuffers();
        myWindow.pollEvents();
    }

    glfwTerminate();
    return 0;
}


#endif