#include "Tester.h"

namespace Tester {

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
		auto p = [](std::string s) {std::cout << s << "\n"; };
		KeyboardMappings km{};
		InputHandler ih{ };
		p("Running Tests");
		ih.bindControlPressed(Control::TurnLeft, [p]() {p("left"); });
		ih.bindControlPressed(Control::TurnLeft, [p]() {p("left1"); });
		ih.bindControlPressed(Control::TurnLeft, [p]() {p("left2"); });
		ih.bindControlPressed(Control::TurnRight, [p]() {p("right"); });
		ih.bindControlPressed(Control::TurnRight, [p]() {p("right1"); });
		ih.bindControlPressed(Control::TurnRight, [p]() {p("right2"); });
		ih.bindControlPressed(Control::LookUp, [p]() {p("up"); });
		ih.bindControlPressed(Control::LookUp, [p]() {p("up1"); });
		ih.bindControlPressed(Control::LookUp, [p]() {p("up2"); });
		ih.bindControlPressed(Control::LookDown, [p]() {p("down"); });
		ih.bindControlPressed(Control::LookDown, [p]() {p("down1"); });
		ih.bindControlPressed(Control::LookDown, [p]() {p("down2"); });
		ih.bindKeyPressed(KeyCode::A, [p]() {p("a pressed"); });
		ih.bindKeyPressed(KeyCode::A, [p]() {p("a pressed 1"); });
		bool success = true;
		std::cout << "===========" << "\n";
		success = success && ih.invokeControlPressed(Control::TurnLeft);
		std::cout << "===========" << "\n";
		success = success && ih.invokeControlPressed(Control::TurnRight);
		std::cout << "===========" << "\n";
		success = success && ih.invokeControlPressed(Control::LookUp);
		std::cout << "===========" << "\n";
		success = success && ih.invokeControlPressed(Control::LookDown);
		std::cout << "===========" << "\n";
		success = success && ih.invokeKeyPressed(KeyCode::A);
		std::cout << "===========" << "\n";
		std::cout << "done" << "\n";

		return success;
	}
}