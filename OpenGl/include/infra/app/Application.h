#pragma once
#include "core/assets/AssetManager.h"
#include "infra/renderer/Renderer.h"
#include "infra/app/Window.h"
#include "core/assets/AssetPipeline.h"
#include "core/input/InputHandler.h"
#include "core/ecs/Entity.h"
#include "infra/app/GlfwKeyHandler.h"
#include "core/engine/Engine.h"
namespace Engine::Infra
{

	class Application
	{
	private:
		Infra::Renderer renderer;
		Engine::Core::Engine engine;
		Infra::GlfwKeyHandler keyHandler;

		Window window{ 720, 480, "OpenGL Window", nullptr, nullptr };
	public:

		Application() = default;
		~Application() = default;
		
		Core::AssetPipeline assetPipeline;

		void setupImportCallbacks();
		void importAssets();

		void setupInput();
		void processInput();
		void updateGameLogic();
		void updateRenderQueue();

		void run();
	};

}