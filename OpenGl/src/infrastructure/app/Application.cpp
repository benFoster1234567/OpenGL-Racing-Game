#include "infra/app/Application.h"
#include "infra/engine/ImportFuncs.h"
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <core/assets/MeshData.h>
#include <core/input/KeyboardInput.h>
#include <core/input/Keys.h>
#include <infra/app/Window.h>
#include <infra/engine/DebugConsoleUI.h>

#include <vector>
#include <iterator>
#include <stdexcept>
#include <core/assets/ShaderData.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/ext/matrix_float4x4.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include <bit>

Engine::Infra::Application::Application()
{
	window = std::make_unique<Window>("window", false);
	setupWindowCallbacks();
	debugConsoleUi = std::make_unique<DebugConsoleUi>(*window);
	setupDebugCommands();
	setupInput();
}

void Engine::Infra::Application::submitEngineRenderQueueToRenderer()
{

}

//all asset import calls go here
void Engine::Infra::Application::importAssets()
{
	engine.assetPipeline.submit<Core::MeshData>("assets/meshes/bunny.obj", "bunny");
	engine.assetPipeline.submit<Core::ShaderData>("assets/shaders/shader.glsl", "shader");
	engine.createAssetManager();
}

//all debug command lambdas are setup here
void Engine::Infra::Application::setupDebugCommands()
{
	//TODO: check for memory leaks with these lambda captures. Ensure they are not called after the window is destroyed.
    std::function <std::string()> exitFunc = [&]()
    {
        window->closeApplication = true;
        return "Exiting...";
    };

	std::function <std::string()> setFullscreen = [&]()
		{
			window->setFullscreen();
			return "set to fullscreen...";
		};

	std::function <std::string()> setWindowed = [&]()
		{
			window->setWindowed();
			return "setting windowed";
		};

	std::function < std::string(int, int)> setWindowSize = [&](int w, int h)
		{
			window->setWindowSize(w, h);
			return "window size set to : [ " + std::to_string(w) + ", " + std::to_string(h) + " ]";
		};

	std::function <std::string(int)> setPolygonMode = [&](int m)
		{
			renderer.setPolygonMode(m);
			std::string mstr = m == LINE ? "line" : "fill";
			return "Polygon mode set to " + mstr;
		};

	debugConsoleUi->registerCommand<>("setWindowed", setWindowed);
	debugConsoleUi->registerCommand<>("setFullscreen", setFullscreen);
	debugConsoleUi->registerCommand<>("exit", exitFunc);
	debugConsoleUi->registerCommand<int>("pMode", setPolygonMode);
}

//window key callback is set here
void Engine::Infra::Application::setupWindowCallbacks()
{
	std::function<void(int, int, int, int)> callback = [&](int _key, int _scancode, int _action, int _mods)
		{
			if (debugConsoleUi->isKeyboardCaptured()) return;
			Engine::Core::KeyAction ka = keyHandler.getAction(_action);
			Engine::Core::KeyCode k = keyHandler.getInput(_key);
			//toggleConsole(k, ka);
			
			if (k == Engine::Core::KeyCode::BackTick && ka == Engine::Core::KeyAction::Down)
			{
				debugConsoleUi->toggleVisibility();
				if (debugConsoleUi->isVisible)
				{
					window->enableCursor();
				}
				else
				{
					window->disableCursor();
				}
				std::cout << "Console Visibility: " << debugConsoleUi->isVisible << "\n";
			}

			else if (_action == GLFW_PRESS)
			{
				engine.inputHandler.setKey(k, true);
			}

			else if (_action == GLFW_RELEASE)
			{
				engine.inputHandler.setKey(k, false);
			}

		};

	auto mouseMotionCallback = [&](double x, double y)
		{
			engine.inputHandler.updateMousePosition({ x,y });
		};
	//window->submitMouseMotionCallback(mouseMotionCallback);
	window->submitKeyCallback(callback);

}

//import callback lambdas are created here
void Engine::Infra::Application::setupImportCallbacks()
{
	engine.assetPipeline.registerImportCallback<Core::MeshData>([](const std::string& path, const std::string& name) -> std::unique_ptr<Core::MeshData>
	{
		return std::make_unique<Core::MeshData>(Infra::ImportFuncs::importMeshData(path, name));
	});

	engine.assetPipeline.registerImportCallback<Core::ShaderData>([](const std::string& path, const std::string& name) -> std::unique_ptr<Core::ShaderData>
	{
		return std::make_unique<Core::ShaderData>(Infra::ImportFuncs::importShaderData(path, name));
	});
}

//TODO: delete this
void Engine::Infra::Application::setupInput()
{

}

void Engine::Infra::Application::run()
{
	//TODO: ensure that all of the dispatchers are reset when this is destroyed.
	engine.shaderDispatcher.subscribe([&](std::vector<Engine::Core::ShaderData*> shaderList) 
		{ 
			renderer.loadShaders(shaderList); 
			std::cout << "load shaders invoked!" << "\n";
		});

	engine.meshDispatcher.subscribe([&](std::vector<Engine::Core::MeshData*> meshList)
		{
			renderer.loadMeshes(meshList);
			std::cout << "load meshes invoked!\n";
		});

	Engine::Core::ECS::RenderDispatcher::sendRenderInfo.subscribe([&](Engine::Core::ECS::RenderOutput output)
		{
			RenderCommand rc = { .view = output.view, .projection = output.projection, .modelTransform = output.modelTransform, .shader = output.shader, .mesh = output.mesh };
			renderer.submit(rc);
		});

	setupImportCallbacks();
	importAssets();
	engine.publishAssets();
	float currentWidth = static_cast<float>(window->getWidth());
	float currentHeight = static_cast<float>(window->getHeight());

	engine.setUpGame();

	while (!window->shouldClose())
	{
		window->pollEvents();
		window->updateViewport();

		renderer.clear();

		window->updateDeltaTime();
		
		float currentWidth = static_cast<float>(window->getWidth());
		float currentHeight = static_cast<float>(window->getHeight());
		
		double x{}, y{};
		window->updateDeltaTime();
		window->getMousePosition(x, y);
		engine.updateAspect(currentWidth / currentHeight);
		engine.updateMouse(x, y);
		engine.updateGame();

		renderer.flush();
		debugConsoleUi->prepareFrame();
		debugConsoleUi->render();
		window->swapBuffers();
	}

	Engine::Core::ECS::RenderDispatcher::sendRenderInfo.clear();

	window->terminateGlfw();
	//delete mesh;

}
