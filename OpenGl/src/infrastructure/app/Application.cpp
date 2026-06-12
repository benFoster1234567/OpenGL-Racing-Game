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

	engine.getAssetImporter().registerImportCallback<Core::ShaderData>([](const std::string& path, const std::string& name) -> std::unique_ptr<Core::ShaderData>
	{
		return std::make_unique<Core::ShaderData>(Infra::ImportFuncs::importShaderData(path, name));
	});
}

void Engine::Infra::Application::importAssets()
{
	engine.getAssetImporter().submit<Core::MeshData>("assets/meshes/bunny.obj", "bunny");
	engine.getAssetImporter().submit<Core::ShaderData>("assets/shaders/shader.glsl", "shader");

	try
	{
		engine.createAssetManager();
	}
	catch (std::runtime_error& error)
	{
		std::cerr << "Caught an error: " << error.what() << std::endl;
	}
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

void Engine::Infra::Application::updateRenderQueue()
{
	auto eList = engine.pollEntities();

	float currentWidth = static_cast<float>(window->getWidth());
	float currentHeight = static_cast<float>(window->getHeight());

	if (currentHeight == 0) currentHeight = 1.0f;

	for (const auto & e : eList)
	{
		RenderCommand rc
		{
			.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			.projection = glm::perspective(glm::radians(45.0f), currentHeight / currentHeight, 0.1f, 100.0f),
			.modelTransform = glm::mat4(1),
			.shader = e.shader,
			.mesh = e.mesh
		};
		renderer.submit(rc);
	}
}

void Engine::Infra::Application::run()
{
	
	//test renderer with a shader...

	const char* shaderSrcRaw = R"glsl(
    #ifdef VERTEX_SHADER

    layout (location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
	#endif

	#ifdef FRAGMENT_SHADER
    
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }

	#endif
		
	)glsl";

	Core::ShaderData* shaderData = new Core::ShaderData{};
	shaderData->name = "newestShader";
	shaderData->shaderSrc = shaderSrcRaw;

	std::vector<Core::ShaderData*> shaders{};
	shaders.push_back(shaderData);

	renderer.loadShaders(shaders);

	//test renderer with a cube model...

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f
	};

	Core::Attribute a{ .data = std::vector<float>(std::begin(vertices), std::end(vertices)), .size = 3, .index = 0 };

	std::vector<Core::Attribute> attr{};
	attr.push_back(a);

	Core::MeshData* mesh = new Core::MeshData(attr);

	std::vector<Core::MeshData*> meshes{};
	meshes.push_back(mesh);

	renderer.loadMeshes(meshes);

	//Create a render command...
	RenderCommand rc
	{
		.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
		.projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f),
		.modelTransform = glm::mat4(1),
		.shader = shaderData,
		.mesh = mesh 
	};
	
	

	float lastFrame = 0.0f;
	float cubeRotation = 0.0f;

	while (!window->shouldClose())
	{
		window->updateViewport();

		float currentWidth = static_cast<float>(window->getWidth());
		float currentHeight = static_cast<float>(window->getHeight());

		if (currentHeight == 0) currentHeight = 1.0f;

		rc.projection = glm::perspective(glm::radians(45.0f), currentWidth / currentHeight, 0.1f, 100.0f);


		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		float currentFrame = static_cast<float>(glfwGetTime());
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		cubeRotation += 50.0f * deltaTime;


		glm::mat4 model(1.0f);
		model = glm::rotate(model, glm::radians(cubeRotation), glm::vec3(0.5f, 1.0f, 0.0f));
		rc.modelTransform = model;

		renderer.submit(rc);
		renderer.flush();

		debugConsoleUi->prepareFrame();

		debugConsoleUi->render();

		window->swapBuffers();
		window->pollEvents();
	}

	window->terminateGlfw();

}
