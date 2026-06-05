#include "infra/engine/DebugConsoleUI.h"

void Engine::Infra::DebugConsoleUi::queueUiDraw()
{
    ImGui::ShowDemoWindow();
}

Engine::Infra::DebugConsoleUi::DebugConsoleUi(Window& window, const std::string& gl_version)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.glfwWindow, true);
    ImGui_ImplOpenGL3_Init(gl_version.c_str());
}

void Engine::Infra::DebugConsoleUi::assembleCommands()
{
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
