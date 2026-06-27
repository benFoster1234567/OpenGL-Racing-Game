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

#include <vector>
#include <iterator>
#include <stdexcept>
#include <core/assets/ShaderData.h>
#include <GLFW/glfw3.h>
//#include <glm/ext/matrix_transform.inl>
#include <glm/ext/matrix_clip_space.inl>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.inl>
#include <GL/glew.h>


Engine::Infra::Application::Application()
{
	window = std::make_unique<Window>("window", false);
	setupWindowKeyCallback();
	debugConsoleUi = std::make_unique<DebugConsoleUi>(*window);
	setupDebugCommands();
	setupInput();
}

static Engine::Infra::RenderCommand createRcFromEntity(Engine::Core::Entity* e, glm::mat4 viewMat, glm::mat4 projMat)
{
	Engine::Infra::RenderCommand rc
	{
		.view = viewMat,
		.projection = projMat,
		.modelTransform = e->getTransformMatrix(),
		.shader = e->shader,
		.mesh = e->mesh
	};
	return rc;
}

void Engine::Infra::Application::submitEngineRenderQueueToRenderer()
{
	//using these for now until I get a camera thing going

	float currentWidth = static_cast<float>(window->getWidth());
	float currentHeight = static_cast<float>(window->getHeight());
	
	auto projection = glm::perspective(glm::radians(45.0f), currentWidth / currentHeight, 0.1f, 100.0f);
	auto view = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	if (currentHeight == 0) currentHeight = 1.0f;



	std::vector<Engine::Core::Entity*> entities{};
	engine.fillEntityRenderList(entities);
	if (entities.empty())
		throw std::runtime_error("entityListBlank!");
	
	for (auto& e : entities)
		renderer.submit(createRcFromEntity(e, view, projection));
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

void Engine::Infra::Application::setupInput()
{
	auto ctrlTrack = [&](Engine::Core::Control c, Engine::Core::KeyAction a)
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

	auto toggleConsole = [&](Engine::Core::KeyCode k, Engine::Core::KeyAction a)
		{
			if (k == Engine::Core::KeyCode::BackTick && a == Engine::Core::KeyAction::Down)
			{
				debugConsoleUi->toggleVisibility();
				std::cout << "Console Visibility: " << debugConsoleUi->isVisible << "\n";
			}
		};

	engine.inputHandler.subscribeControl(ctrlTrack);
	engine.inputHandler.subscribeKey(toggleConsole);
}

void Engine::Infra::Application::run()
{
	
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

	setupImportCallbacks();
	
	importAssets();
	engine.dispatchAssets();
	engine.createEntities();

	
	float lastFrame = 0.0f;
	float cubeRotation = 0.0f;

	while (!window->shouldClose())
	{
		window->updateViewport();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		float currentFrame = static_cast<float>(glfwGetTime());
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		engine.setDeltaTime(deltaTime);
		engine.updateRenderQueue();

		submitEngineRenderQueueToRenderer();
		renderer.flush();

		debugConsoleUi->prepareFrame();
		debugConsoleUi->render();

		window->swapBuffers();
		window->pollEvents();
	}

	window->terminateGlfw();
	//delete mesh;

}
