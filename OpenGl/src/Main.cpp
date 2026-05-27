#ifndef ENABLE_TESTS

#include "infra/app/Window.h"

int main()
{
    if (!glfwInit()) { return -1; }

    Engine::Infra::Window myWindow(800, 600, "Test Window", nullptr, nullptr);

    while (!myWindow.shouldClose())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        myWindow.swapBuffers();
        myWindow.pollEvents();
    }

    glfwTerminate();
    return 0;
}


#endif