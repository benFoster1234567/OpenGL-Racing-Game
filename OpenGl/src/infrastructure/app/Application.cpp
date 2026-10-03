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
#include <infra/renderer/GpuAssetLoader.h>
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
	m_window = std::make_unique<Window>("window", false);
	setupWindowCallbacks();
	m_debugConsoleUi = std::make_unique<DebugConsoleUi>(*m_window);
	setupDebugCommands();
}


//all asset import calls go here
void Engine::Infra::Application::importAssets()
{
	m_engine.m_assetPipeline.submit<Core::MaterialData>("assets/materials/testMaterial.mtl", "testMaterial");
	m_engine.m_assetPipeline.submit<Core::MaterialData>("assets/materials/cubeMaterial.mtl", "cubeMaterial");
	m_engine.m_assetPipeline.submit<Core::ShaderData>("assets/shaders/depthCubeShader.glsl", "depthCubemap");
	m_engine.m_assetPipeline.submit<Core::MeshData>("assets/meshes/bunny.obj", "bunny");
	m_engine.m_assetPipeline.submit<Core::MeshData>("assets/meshes/cube.obj", "cube");
	m_engine.m_assetPipeline.submit<Core::MeshData>("assets/meshes/car.obj", "car");
	m_engine.m_assetPipeline.submit<Core::ShaderData>("assets/shaders/shader.glsl", "shader");
	m_engine.m_assetPipeline.submit<Core::TextureData>("assets/materials/textures/testTextures.jpg", "uvChecker");
	m_engine.m_assetPipeline.submit<Core::TextureData>("assets/materials/textures/gold.jpg", "gold");
	m_engine.m_assetPipeline.submit<Core::TextureData>("assets/materials/textures/pic0068.gif", "tileSpecular");
	m_engine.m_assetPipeline.submit<Core::TextureData>("assets/materials/textures/pic0066.gif", "tileDiffuse");
	m_engine.m_assetPipeline.submit<Core::TextureData>("assets/materials/textures/pic0067.gif", "tileNormal");
	m_engine.createAssetManager();
}


//all debug command lambdas are setup here
void Engine::Infra::Application::setupDebugCommands()
{
	std::function <std::string()> exitFunc = [&]()
		{
			m_window->m_closeApplication = true;
			return "Exiting...";
		};

	std::function <std::string()> setFullscreen = [&]()
		{
			m_window->setFullscreen();
			return "set to fullscreen...";
		};

	std::function <std::string()> setWindowed = [&]()
		{
			m_window->setWindowed();
			return "setting windowed";
		};

	std::function < std::string(int, int)> setWindowSize = [&](int w, int h)
		{
			m_window->setWindowSize(w, h);
			return "window size set to : [ " + std::to_string(w) + ", " + std::to_string(h) + " ]";
		};

	std::function <std::string(int)> setPolygonMode = [&](int m)
		{
			m_renderer.setPolygonMode(m);
			std::string mstr = m == LINE ? "line" : "fill";
			return "Polygon mode set to " + mstr;
		};

	m_debugConsoleUi->registerCommand<>("setWindowed", setWindowed);
	m_debugConsoleUi->registerCommand<>("setFullscreen", setFullscreen);
	m_debugConsoleUi->registerCommand<>("exit", exitFunc);
	m_debugConsoleUi->registerCommand<int>("pMode", setPolygonMode);
}

//window key callback is set here
void Engine::Infra::Application::setupWindowCallbacks()
{
	std::function<void(int, int, int, int)> callback = [&](int _key, int _scancode, int _action, int _mods)
		{
			if (m_debugConsoleUi->isKeyboardCaptured()) return;
			Engine::Core::KeyAction ka = m_keyHandler.getAction(_action);
			Engine::Core::KeyCode k = m_keyHandler.getInput(_key);
			//toggleConsole(k, ka);

			if (k == Engine::Core::KeyCode::BackTick && ka == Engine::Core::KeyAction::Down)
			{
				m_debugConsoleUi->toggleVisibility();
				if (m_debugConsoleUi->isVisible)
				{
					m_window->enableCursor();
				}
				else
				{
					m_window->disableCursor();
				}
				std::cout << "Console Visibility: " << m_debugConsoleUi->isVisible << "\n";
			}

			else if (_action == GLFW_PRESS)
			{
				m_engine.m_inputHandler.setKey(k, true);
			}

			else if (_action == GLFW_RELEASE)
			{
				m_engine.m_inputHandler.setKey(k, false);
			}

		};

	auto mouseMotionCallback = [&](double x, double y)
		{
			m_engine.m_inputHandler.updateMousePosition({ x,y });
		};
	//window->submitMouseMotionCallback(mouseMotionCallback);
	m_window->submitKeyCallback(callback);

}

//import callback lambdas are created here
void Engine::Infra::Application::setupImportCallbacks()
{
	m_engine.m_assetPipeline.registerImportCallback<Core::MeshData>([](const std::string& path, const std::string& name) -> std::unique_ptr<Core::MeshData>
		{
			return std::make_unique<Core::MeshData>(Infra::ImportFuncs::importMeshDataTOL(path, name));
		});

	m_engine.m_assetPipeline.registerImportCallback<Core::ShaderData>([](const std::string& path, const std::string& name) -> std::unique_ptr<Core::ShaderData>
		{
			return std::make_unique<Core::ShaderData>(Infra::ImportFuncs::importShaderData(path, name));
		});

	m_engine.m_assetPipeline.registerImportCallback<Core::MaterialData>([](const std::string& path, const std::string& name) -> std::unique_ptr<Core::MaterialData>
		{
			return std::make_unique<Core::MaterialData>(Infra::ImportFuncs::importMaterialData(path, name));
		});

	m_engine.m_assetPipeline.registerImportCallback<Core::TextureData>([](const std::string& path, const std::string& name) -> std::unique_ptr<Core::TextureData>
		{
			return std::make_unique<Core::TextureData>(Infra::ImportFuncs::importTextureDataDevIL(path, name));
		});
}

void Engine::Infra::Application::run()
{

	Engine::Core::ECS::RenderDispatcher::sendRenderInfo.subscribe([&](Engine::Core::ECS::RenderOutput output)
		{
			RenderCommand rc = { .view = output.view, .projection = output.projection, .modelTransform = output.modelTransform, .uvScale = output.uvScale, .shader = output.shader, .mesh = output.mesh,.material = output.material };
			m_renderer.submit(rc);
		});

	m_renderer.setPolygonMode(0);

	setupImportCallbacks();
	importAssets();

	GpuAssetLoader::fillRenderer(m_engine.m_assetManager, m_renderer);

	{
		std::vector<Engine::Core::ECS::StaticPointLightRendererData> lightData{};
		std::vector<StaticPointLightResource> pointLights{};

		//TODO: Tidy this up
		m_engine.setUpGame();
		m_engine.fillStaticLightVector(lightData);

		for (const auto& light : lightData)
		{
			StaticPointLightResource splr{};
			splr.position = light.m_position;
			splr.color = light.m_color;
			splr.radius = light.radius;
			splr.intensity = light.intensity;
			pointLights.push_back(splr);
		}

		m_renderer.loadLights(pointLights);
	}

	auto getCameraOrigin = [this]() -> glm::vec3
		{
			return m_engine.m_game.m_coordinator.getComponent<Core::ECS::TransformComponent>(m_engine.m_game.m_playerEntity).position;
		};

	m_renderer.loadShadowingLights(getCameraOrigin());

	glfwSwapInterval(0);
	m_window->disableCursor();
	m_renderer.prepareDepthCubemapArray();

	float physicsUpdateSeconds = 0.02f;
	float physicsUpdateDuration = 0.0f;

	m_btPhysicsEngine.loadPhysicsCommands(m_engine.getPhysicsCommandQueue());
	m_btPhysicsEngine.evaluateCommands();
	m_btPhysicsEngine.createVehicles(m_engine.getCoordinator());

	while (!m_window->shouldClose())
	{
		m_window->pollEvents();
		m_window->updateViewport();
		m_window->updateDeltaTime();
		float deltaTime = m_window->deltaTime();

		float currentWidth = static_cast<float>(m_window->getWidth());
		float currentHeight = static_cast<float>(m_window->getHeight());
		double x{}, y{};

		m_window->getMousePosition(x, y);

		m_engine.updateAspect(currentWidth / currentHeight);
		m_engine.updateDeltaTime(deltaTime);
		m_engine.updateMouse(x, y);
		m_engine.updateGame();
		m_renderer.updatePointlightSources(getCameraOrigin());

		m_btPhysicsEngine.updateVehicles(m_engine.getCoordinator());
		m_btPhysicsEngine.runSimulation(deltaTime);

		m_engine.pollPhysicsEvents(m_btPhysicsEngine.pollEvents());
		m_engine.zeroMouse();

		m_renderer.flush(currentWidth, currentHeight);
		//renderer.renderLights();
		m_debugConsoleUi->prepareFrame();
		m_debugConsoleUi->render();

		m_window->swapBuffers();
	}

	Engine::Core::ECS::RenderDispatcher::sendRenderInfo.clear();

	m_window->terminateGlfw();
	//delete mesh;

}
