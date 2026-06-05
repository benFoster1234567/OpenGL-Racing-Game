#include "infra/app/Application.h"
#include "infra/engine/ImportFuncs.h"

Engine::Infra::Application::Application()
{
	auto vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	auto width = vidMode->width;
	auto height = vidMode->height;
	auto monitor = glfwGetPrimaryMonitor();
	window = std::make_unique<Window>(width, height, "OpenGL Engine", monitor, nullptr);
	debugConsoleUi = std::make_unique<DebugConsoleUi>(*window);
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

void Engine::Infra::Application::setupImportCallbacks()
{
	engine.getAssetImporter().registerImportCallback<Core::MeshData>([](const std::string& path, const std::string& name) -> std::unique_ptr<Core::MeshData>
		{
			return std::make_unique<Core::MeshData>(Infra::ImportFuncs::importMeshData(path, name));
		});
}

void Engine::Infra::Application::importAssets()
{

}

void Engine::Infra::Application::setupInput()
{

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
}
