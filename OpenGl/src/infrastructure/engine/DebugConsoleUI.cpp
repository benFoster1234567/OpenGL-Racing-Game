#include "infra/engine/DebugConsoleUI.h"
#include <iostream>

void Engine::Infra::DebugConsoleUi::queueUiDraw()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Always);
	ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	
	if (ImGui::InputText("##input", commandBuffer, sizeof(commandBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{

		try
		{
			std::string result = debugConsole.executeCommand(commandBuffer);
			std::cout << "Command result: " << result << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error executing command: " << e.what() << std::endl;
		}
		memset(commandBuffer, 0, sizeof(commandBuffer));
		ImGui::SetKeyboardFocusHere(-1);

	}

	ImGui::End();
}

Engine::Infra::DebugConsoleUi::DebugConsoleUi(Window& window, const std::string& gl_version)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.glfwWindow, true);
    ImGui_ImplOpenGL3_Init(gl_version.c_str());
	assembleCommands();
}

void Engine::Infra::DebugConsoleUi::assembleCommands()
{
	std::function <std::string(int, int)> func = [](int x, int y) { return std::to_string(x + y); };
	debugConsole.registerCommand<int, int>("add", func);
}

void Engine::Infra::DebugConsoleUi::prepareFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
    queueUiDraw();
	ImGui::Render();
}

void Engine::Infra::DebugConsoleUi::render()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::vector<std::string> Engine::Infra::DebugConsole::tokenize(const std::string& input)
{
	std::stringstream ss(input);
	std::string token;
	std::vector<std::string> tokens;

	while (ss >> token) {
		tokens.push_back(token);
	}

	return tokens;
}

std::string Engine::Infra::DebugConsole::executeCommand(const std::string& consoleInput)
{
	auto tokens = tokenize(consoleInput);

	if (tokens.empty())
	{
		throw std::runtime_error("No command entered.");
	}

	auto it0 = tokens.begin();
	std::string name = *it0++;

	auto it = commandRegistry.find(name);

	if (it == commandRegistry.end())
	{
		throw std::runtime_error("Command not found: " + name);
	}

	return it->second->execute({ it0, tokens.end() });
}
