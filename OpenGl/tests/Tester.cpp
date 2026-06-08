#include "Tester.h"

namespace Tester {
	using namespace Engine::Core;
	void printSuccessMessage(bool success, const std::string& successMsg, const std::string& failureMsg, std::ostream& out)
	{
		out << (success ? successMsg : failureMsg) << "\n";

	}

	void printSuccessMessage(bool success, std::ostream& out)
	{
		out << (success ? "test success !" : "test failure !") << "\n";
	}



	bool testInputHandler()
	{
		

		return true;
	}

	bool testDebugConsole1()
	{
		Engine::Infra::DebugConsole dc{};
		std::function <std::string(int, int)> func = [](int x, int y) { return std::to_string(x + y); };
		dc.registerCommand<int, int>("echo", func);
		return dc.executeCommand("echo 3 4") == "7";
	}

	bool testDebugConsole2()
	{
		Engine::Infra::DebugConsole dc{};
		std::function <std::string(std::string, int)> func = [](std::string x, int y) { return std::to_string(std::stoi(x) + y); };
		dc.registerCommand<std::string, int>("echo", func);
		return dc.executeCommand("echo 3 4") == "7";
	}
}