//#define ENABLE_TESTS
#ifndef ENABLE_TESTS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "infra/app/Application.h"

int main()
{
    if (!glfwInit()) { return -1; }
	Engine::Infra::Application app{};
	app.run();
    return 0;
}


#endif