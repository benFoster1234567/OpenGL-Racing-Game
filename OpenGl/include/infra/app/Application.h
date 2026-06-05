#pragma once
#include "core/assets/AssetManager.h"
#include "infra/renderer/Renderer.h"
#include "infra/app/Window.h"
#include "core/assets/AssetPipeline.h"
#include "core/input/InputHandler.h"
#include "core/ecs/Entity.h"
#include "infra/app/GlfwKeyHandler.h"
#include "core/engine/Engine.h"
#include "infra/engine/DebugConsoleUI.h"
namespace Engine::Infra
{

	class Application
	{
	private:
		Infra::Renderer renderer;
		Engine::Core::Engine engine;
		Infra::GlfwKeyHandler keyHandler;
		std::unique_ptr<DebugConsoleUi> debugConsoleUi;
		std::unique_ptr<Window> window;
	public:

		Application();
		~Application() = default;
		
		Core::AssetPipeline assetPipeline;
		
		void setupDebugCommands();
		void setupImportCallbacks();
		void importAssets();

		void setupInput();
		void processInput();
		void updateGameLogic();
		void updateRenderQueue();

		void run();
	};

}