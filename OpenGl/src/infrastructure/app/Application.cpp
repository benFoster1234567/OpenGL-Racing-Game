#include "infra/app/Application.h"
#include "infra/engine/ImportFuncs.h"
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <core/assets/MeshData.h>
#include <core/input/InputHandler.h>
#include <core/input/Keys.h>
#include <infra/app/Window.h>
#include <infra/engine/DebugConsoleUI.h>

Engine::Infra::Application::Application()
{
	window = std::make_unique<Window>("window", true);
	setupWindowKeyCallback();
	debugConsoleUi = std::make_unique<DebugConsoleUi>(*window);
	setupDebugCommands();
	setupInput();
}

void Engine::Infra::Application::setupDebugCommands()
{
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

	debugConsoleUi->registerCommand<>("setWindowed", setWindowed);
	debugConsoleUi->registerCommand<>("setFullscreen", setFullscreen);
	debugConsoleUi->registerCommand<>("exit", exitFunc);
}

void Engine::Infra::Application::setupWindowKeyCallback()
{
	std::function<void(int, int, int, int)> callback = [&](int _key, int _scancode, int _action, int _mods)
		{
			if (debugConsoleUi->isKeyboardCaptured()) return;
			Engine::Core::KeyAction ka = keyHandler.getAction(_action);
			Engine::Core::KeyCode k = keyHandler.getInput(_key);
			engine.inputHandler.invokeKeyAndControl(k, ka);

		};
	window->submitKeyCallback(callback);
}

void Engine::Infra::Application::setupImportCallbacks()
{
	engine.getAssetImporter().registerImportCallback<Core::MeshData>([](const std::string& path, const std::string& name) -> std::unique_ptr<Core::MeshData>
	{
		return std::make_unique<Core::MeshData>(Infra::ImportFuncs::importMeshData(path, name));
	});
}

void Engine::Infra::Application::importAssets()
{
	engine.getAssetImporter().submit<Core::MeshData>("bunny.obj", "bunny");
	engine.createAssetManager();
}

void Engine::Infra::Application::setupInput()
{
	auto keyTrack = [&](Engine::Core::Control c, Engine::Core::KeyAction a)
		{
			switch (c)
			{
			case Engine::Core::Control::TurnLeft:
				std::cout << "Turn Left"; break;
			case Engine::Core::Control::TurnRight:
				std::cout << "Turn Right"; break;
			default:
				std::cout << "Control Active"; break;
			}

			switch (a)
			{
			case Engine::Core::KeyAction::Down: std::cout << " Down"; break;
			case Engine::Core::KeyAction::Up: std::cout << " Up"; break;
			case Engine::Core::KeyAction::Held: std::cout << " Held"; break;
			}
			std::cout << "\n";
		};

	engine.inputHandler.subscribeControl(keyTrack);

}

void Engine::Infra::Application::updateRenderQueue()
{

}

void Engine::Infra::Application::run()
{

    while (!window->shouldClose())
    {
		renderer.clearColor();
        window->updateViewport();
        debugConsoleUi->prepareFrame();
        debugConsoleUi->render();
        window->swapBuffers();
        window->pollEvents();
    }
	window->terminateGlfw();
}
