#pragma once
#include "Core/engine/Engine.h"

namespace Engine::Infra
{

	class ApplicationBase
	{
	private:


	public:
		ApplicationBase() = default;
		~ApplicationBase() = default;
		ApplicationBase(const ApplicationBase&) = delete;
		ApplicationBase& operator=(const ApplicationBase&) = delete;
		ApplicationBase(ApplicationBase&&) = default;
		ApplicationBase& operator=(ApplicationBase&&) = default;
		void run();
		virtual void initialize() = 0;
		virtual void shutdown() = 0;
		virtual void updateGameLoop() = 0;
		virtual void updateRenderLoop() = 0;
	};

	class Application : private ApplicationBase
	{
	private:
		Core::Engine engine;
	public:
		Application() = default;
		~Application() = default;
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = default;
		Application& operator=(Application&&) = default;

		void initialize() override
		{

		}

	};
}