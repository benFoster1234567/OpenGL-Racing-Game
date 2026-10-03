#include "infra/engine/DebugConsoleUI.h"
#include <iostream>

void Engine::Infra::DebugConsoleUi::queueUiDraw()
{
	if (!isVisible) return;

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

	if (ImGui::InputText("##input", m_commandBuffer, sizeof(m_commandBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		try
		{
			std::string result = m_debugConsole.executeCommand(m_commandBuffer);
			std::cout << "Command result: " << result << std::endl;
			appendResults(result);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error executing command: " << e.what() << std::endl;
		}
		memset(m_commandBuffer, 0, sizeof(m_commandBuffer));
		ImGui::SetKeyboardFocusHere(-1);

	}

	ImGui::BeginChild("Scrolling");
	for (const auto& s : m_consoleResults)
		ImGui::TextUnformatted(s.c_str());

	ImGui::EndChild();

	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.0f); // Make background transparent

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin("CornerOverlay", nullptr, flags)) {
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	}
	ImGui::End();

}

void Engine::Infra::DebugConsoleUi::appendResults(std::string r)
{
	if (m_consoleResults.size() >= m_resultListMaxSize)
	{
		m_consoleResults.pop_back();
	}
	if (r.empty() || r == "") return;
	m_consoleResults.push_front(r);
}

Engine::Infra::DebugConsoleUi::DebugConsoleUi(Window& window, const std::string& gl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.m_glfwWindow, true);
	ImGui_ImplOpenGL3_Init(gl_version.c_str());
	assembleCommands();
}

bool Engine::Infra::DebugConsoleUi::isKeyboardCaptured()
{
	ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureKeyboard;
}

void Engine::Infra::DebugConsoleUi::assembleCommands()
{
	std::function <std::string(int, int)> func = [](int x, int y) { return std::to_string(x + y); };
	m_debugConsole.registerCommand<int, int>("add", func);
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
