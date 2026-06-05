#pragma once

#include <GLFW/glfw3.h>
#include "infra/app/Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <utility>
namespace Engine::Infra
{

	class CommandBase 
	{
	public:
		virtual std::string execute(const std::vector<std::string>& args) = 0;
		virtual ~CommandBase() = default;
	};

	template <typename... Args>
	class Command : public CommandBase
	{
	private:
		std::string name;
		std::function<std::string(Args...)> commandFunction;
		std::vector<std::string> argumentNames;


		std::string getStringAt(const std::vector<std::string>& argStrs, std::size_t I) {
			return argStrs[I];
		}

		template <typename T>
		T parseArgument(const std::string& argStr)
		{
			std::stringstream ss(argStr);
			T value;
			
			if (!(ss >> value)) {
				throw std::runtime_error("Failed to parse argument: " + argStr);
			}

			return value;
		}

		template<std::size_t... Is>
		std::string executeImpl(const std::vector<std::string>& argStrs, std::index_sequence<Is...>)
		{
			return commandFunction(parseArgument<Args>(getStringAt(argStrs, Is))...);
		}


	public:
		Command(const std::string& name, std::function<std::string(Args...)> func)
			: name{ name }, commandFunction{ func }
		{
		}

		Command(const Command&) = default;
		Command(Command&&) = default;
		~Command() = default;



		std::string execute(const std::vector<std::string>& inputArgs) override
		{
			if (inputArgs.size() != sizeof...(Args)) {
				throw std::runtime_error("Incorrect number of arguments provided to command: " + name);
			}

			return executeImpl(inputArgs, std::index_sequence_for<Args...>{});
		}

	};


	class DebugConsole
	{
	private:
		std::unordered_map<std::string, std::unique_ptr<CommandBase>> commandRegistry{};

		std::vector<std::string> tokenize(const std::string& input);

	public:
		DebugConsole() = default;
		DebugConsole(const DebugConsole&) = delete;
		DebugConsole(DebugConsole&&) = default;
		~DebugConsole() = default;
		std::string executeCommand(const std::string& consoleInput);

		template <typename... Args>
		void registerCommand(const std::string& name, std::function<std::string(Args...)> func)
		{
			commandRegistry[name] = std::make_unique<Command<Args...>>(
				name,
				func
			);
		}
	
	};

	class DebugConsoleUi
	{
	private:
		void queueUiDraw();
		DebugConsole debugConsole;
	public:
		DebugConsoleUi(Window& window, const std::string& gl_version = "#version 430");
		


		~DebugConsoleUi() = default;

		void assembleCommands();

		void prepareFrame();
		void render();


	};
}