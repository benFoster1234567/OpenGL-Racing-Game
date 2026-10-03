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
	: m_width(_width)
	, m_height(_height)
	, m_windowTitle(_windowTitle.c_str())
	, m_isFullscreen(_isFullscreen)
{
	saveWindowState(_x, _y, _width, _height);

	if (!glfwInit())
	{
		throw std::runtime_error("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (m_isFullscreen)
	{
		auto vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		m_width = vidMode->width;
		m_height = vidMode->height;
		auto monitor = glfwGetPrimaryMonitor();
		m_glfwWindow = glfwCreateWindow(m_width, m_height, _windowTitle.c_str(), monitor, nullptr);
	}

	else
	{
		m_glfwWindow = glfwCreateWindow(m_width, m_height, m_windowTitle, nullptr, nullptr);
	}

	if (!m_glfwWindow)
	{
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(m_glfwWindow);

	glfwSetFramebufferSizeCallback(m_glfwWindow, [](GLFWwindow* win, int w, int h)
		{
			glViewport(0, 0, w, h);
		});



	int initialWidth, initialHeight;
	glfwGetFramebufferSize(m_glfwWindow, &initialWidth, &initialHeight);
	glViewport(0, 0, initialWidth, initialHeight);
	glEnable(GL_MULTISAMPLE);
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "GLEW Initialization Failed: " << glewGetErrorString(err) << "\n";
		return;
	}
	glfwSetWindowUserPointer(m_glfwWindow, this);

	glfwSetCursorPosCallback(m_glfwWindow, glfwMouseMotionCallback);


}

void Engine::Infra::Window::saveWindowState(int x, int y, int w, int h)
{
	m_savedWindowState.x = x;
	m_savedWindowState.y = y;
	m_savedWindowState.h = h;
	m_savedWindowState.w = w;
}

void Engine::Infra::Window::saveWindowState()
{
	glfwGetWindowPos(m_glfwWindow, &m_savedWindowState.x, &m_savedWindowState.y);
	glfwGetWindowSize(m_glfwWindow, &m_savedWindowState.w, &m_savedWindowState.h);
}

Window::~Window()
{
	destroyWindow();
}

//We submit this in the application, since the applicaation is what owns the inputhandler.
void Window::submitKeyCallback(std::function<void(int, int, int, int)> callback)
{
	m_keyPressedDispatcher.subscribe(callback);
	glfwSetKeyCallback(m_glfwWindow, glfwKeyCallback);
	glfwSetWindowUserPointer(m_glfwWindow, this);
}

void Engine::Infra::Window::enableCursor()
{
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Engine::Infra::Window::disableCursor()
{
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

std::tuple<float, float> Engine::Infra::Window::getCurrentCursor()
{

	return m_cursorState;
}

void Engine::Infra::Window::setFullscreen()
{
	m_isFullscreen = true;
	glfwGetWindowPos(m_glfwWindow, &m_savedWindowState.x, &m_savedWindowState.y);
	glfwGetWindowSize(m_glfwWindow, &m_savedWindowState.w, &m_savedWindowState.h);
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowMonitor(m_glfwWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}

void Engine::Infra::Window::setWindowed()
{
	m_isFullscreen = false;
	glfwSetWindowMonitor(m_glfwWindow, nullptr, m_savedWindowState.x, m_savedWindowState.y, m_savedWindowState.w, m_savedWindowState.h, 0);
}

void Engine::Infra::Window::setWindowSize(int w, int h)
{
	if (m_isFullscreen)
	{
		return; //maybe fix in the future so I can change the resolution or something
	}

	else
	{
		glfwGetWindowPos(m_glfwWindow, &m_savedWindowState.x, &m_savedWindowState.y);
		glfwGetWindowSize(m_glfwWindow, &m_savedWindowState.w, &m_savedWindowState.h);
		glfwSetWindowMonitor(m_glfwWindow, nullptr, m_savedWindowState.x, m_savedWindowState.y, w, h, 0);
	}

}

void Engine::Infra::Window::updateDeltaTime()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	m_times.deltaTime = currentFrame - m_times.lastTime;
	m_times.lastTime = currentFrame;

	static float interval = 0.0f;

	interval += m_times.deltaTime;

	if (interval >= 5.0f)
	{

		float deltaT = m_times.deltaTime != 0 ? m_times.deltaTime : 1.0f;

		std::cout << "fps: " << 1 / deltaT << "\n";
		interval = 0.0f;
	}
}

void Window::onKey(int key, int scancode, int action, int mods)
{
	m_keyPressedDispatcher.invoke(key, scancode, action, mods);
}

void Engine::Infra::Window::onMouseMotion(double xpos, double ypos)
{
	//std::cout << "mouse motion detected\n";
	m_mouseMotionDispatcher.invoke(xpos, ypos);
}

void Engine::Infra::Window::getMousePosition(double& xpos, double& ypos)
{
	glfwGetCursorPos(m_glfwWindow, &xpos, &ypos);
}

void Engine::Infra::Window::submitMouseMotionCallback(std::function<void(double, double)> func)
{
	m_mouseMotionDispatcher.subscribe(func);
}

