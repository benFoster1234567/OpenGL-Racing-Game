#pragma once
#include "core/assets/AssetManager.h"
#include "infra/renderer/Renderer.h"
#include "infra/app/Window.h"
#include "core/assets/AssetPipeline.h"
#include "infra/app/GlfwKeyHandler.h"
#include "core/engine/Engine.h"
#include "infra/engine/DebugConsoleUI.h"
#include "../bullet/BtPhysicsEngine.h"

namespace Engine::Infra
{

	class Application
	{
	private:
		Infra::Renderer m_renderer;
		Infra::GlfwKeyHandler m_keyHandler;
		std::unique_ptr<DebugConsoleUi> m_debugConsoleUi;
		std::unique_ptr<Window> m_window;

		BtPhysicsEngine m_btPhysicsEngine{};

	public:
		Application();
		~Application() = default;

		Engine::Core::EngineSystem m_engine;

		void setupDebugCommands();
		void setupWindowCallbacks();
		void setupImportCallbacks();

		void importAssets();
		void sendTexturesToRenderer();
		void run();

	};

}