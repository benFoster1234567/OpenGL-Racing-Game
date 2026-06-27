#pragma once
#include "core/assets/AssetManager.h"
#include "infra/renderer/Renderer.h"
#include "infra/app/Window.h"
#include "core/assets/AssetPipeline.h"
#include "infra/app/GlfwKeyHandler.h"
#include "core/engine/Engine.h"
#include "infra/engine/DebugConsoleUI.h"

namespace Engine::Infra
{

	class Application
	{
	private:
		Infra::Renderer renderer;
		Infra::GlfwKeyHandler keyHandler;
		std::unique_ptr<DebugConsoleUi> debugConsoleUi;
		std::unique_ptr<Window> window;

	public:

		Application();
		~Application() = default;
		
		Engine::Core::EngineSystem engine;
		
		void setupDebugCommands();
		void setupWindowKeyCallback();
		void setupImportCallbacks();
		void importAssets();
		void submitEngineRenderQueueToRenderer();
		void setupInput();

		void run();
	};

}