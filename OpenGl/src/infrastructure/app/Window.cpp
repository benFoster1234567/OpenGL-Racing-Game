#include "infra/app/Window.h"
#include <stdexcept>
#include <functional>
#include <string>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace Engine::Infra;

Engine::Infra::Window::Window(
	const std::string& _windowTitle
	, bool _isFullscreen
	, int _width
	, int _height
	, int _x
	, int _y
)
	: width(_width)
	, height(_height)
	, windowTitle(_windowTitle.c_str())
	, isFullscreen(_isFullscreen)
{
	saveWindowState(_x, _y, _width, _height);
	
	if (!glfwInit())
	{
		throw std::runtime_error("Failed to initialize GLFW");
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (isFullscreen)
	{
		auto vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		width = vidMode->width;
		height = vidMode->height;
		auto monitor = glfwGetPrimaryMonitor();
		glfwWindow = glfwCreateWindow(width, height, _windowTitle.c_str(), monitor, nullptr);
	}

	else
	{
		glfwWindow = glfwCreateWindow(width, height, windowTitle, nullptr, nullptr);
	}

	if (!glfwWindow)
	{
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(glfwWindow);

	glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow* win, int w, int h) 
	{
		glViewport(0, 0, w, h);
	});
	


	int initialWidth, initialHeight;
	glfwGetFramebufferSize(glfwWindow, &initialWidth, &initialHeight);
	glViewport(0, 0, initialWidth, initialHeight);

	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "GLEW Initialization Failed: " << glewGetErrorString(err) << "\n";
		return;
	}
	glfwSetWindowUserPointer(glfwWindow, this);
	auto mouseCallback = [](GLFWwindow* window, double xpos, double ypos)
		{
			auto* app = static_cast<Window*>(glfwGetWindowUserPointer(window));
			if (app) 
			{
				app->cursorState = { xpos, ypos };
			}
		};

	glfwSetCursorPosCallback(glfwWindow, glfwMouseMotionCallback);


}

void Engine::Infra::Window::saveWindowState(int x, int y, int w, int h)
{
	savedWindowState.x = x;
	savedWindowState.y = y;
	savedWindowState.h = h;
	savedWindowState.w = w;
}

void Engine::Infra::Window::saveWindowState()
{
	glfwGetWindowPos(glfwWindow, &savedWindowState.x, &savedWindowState.y);
	glfwGetWindowSize(glfwWindow, &savedWindowState.w, &savedWindowState.h);
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

void Engine::Infra::Window::enableCursor()
{
	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void Engine::Infra::Window::disableCursor()
{
	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

std::tuple<float, float> Engine::Infra::Window::getCurrentCursor()
{

	return cursorState;
}

void Engine::Infra::Window::setFullscreen()
{
	isFullscreen = true;
	glfwGetWindowPos(glfwWindow, &savedWindowState.x, &savedWindowState.y);
	glfwGetWindowSize(glfwWindow, &savedWindowState.w, &savedWindowState.h);
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor); 
	glfwSetWindowMonitor(glfwWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}

void Engine::Infra::Window::setWindowed()
{
	isFullscreen = false;
	glfwSetWindowMonitor(glfwWindow, nullptr, savedWindowState.x, savedWindowState.y, savedWindowState.w, savedWindowState.h, 0);
}

void Engine::Infra::Window::setWindowSize(int w, int h)
{
	if (isFullscreen)
	{
		return; //maybe fix in the future so I can change the resolution or something
	}

	else
	{
		glfwGetWindowPos(glfwWindow, &savedWindowState.x, &savedWindowState.y);
		glfwGetWindowSize(glfwWindow, &savedWindowState.w, &savedWindowState.h);
		glfwSetWindowMonitor(glfwWindow, nullptr, savedWindowState.x, savedWindowState.y, w, h, 0);
	}

}

void Window::onKey(int key, int scancode, int action, int mods)
{
	keyPressedDispatcher.invoke(key, scancode, action, mods);
}

void Engine::Infra::Window::onMouseMotion(double xpos, double ypos)
{
	mouseMotionDispatcher.invoke(xpos, ypos);
}

void Engine::Infra::Window::submitMouseMotionCallback(std::function<void(double, double)> func)
{
	mouseMotionDispatcher.subscribe(func);
}

