//#define ENABLE_TESTS
#ifndef ENABLE_TESTS

#include <GL/glew.h>
#include "infra/app/Window.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "infra/engine/DebugConsoleUI.h"
#include "infra/app/Application.h"
int main()
{
    if (!glfwInit()) { return -1; }
	Engine::Infra::Application app{};
	app.run();
    return 0;
}


#endif