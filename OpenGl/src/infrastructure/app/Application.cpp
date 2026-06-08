#include "infra/app/Application.h"
#include "infra/engine/ImportFuncs.h"

Engine::Infra::Application::Application()
{
	auto vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	auto width = vidMode->width;
	auto height = vidMode->height;
	auto monitor = glfwGetPrimaryMonitor();
	window = std::make_unique<Window>(width, height, "OpenGL Engine", monitor, nullptr);
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
	auto keyTrack = [&](Engine::Core::Control c)
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
		};

	auto keyTrackPressed = [&](Engine::Core::Control c)
		{
			keyTrack(c);
			std::cout << " Pressed\n";
		};

	auto keyTrackHeld = [&](Engine::Core::Control c)
		{
			keyTrack(c);
			std::cout << " Held\n";
		};

	auto keyTrackReleased = [&](Engine::Core::Control c)
		{
			keyTrack(c);
			std::cout << " Released\n";
		};

	engine.inputHandler.subscribeControlPressed(keyTrackPressed);
	engine.inputHandler.subscribeControlReleased(keyTrackReleased);
	engine.inputHandler.subscribeControlHeld(keyTrackHeld);
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
