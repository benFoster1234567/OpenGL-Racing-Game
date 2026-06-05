#pragma once
#include "core/events/EventDispatcher.h"
#include "core/input/InputHandler.h"
#include "infra/engine/DebugConsoleUI.h"


#include <iostream>
#include <string>


namespace Tester
{
	void printSuccessMessage(bool success, const std::string& successMsg = "test passed ! "
		, const std::string& failureMsg = "test failed !", std::ostream& out = std::cout);

	bool testInputHandler();
	bool testDebugConsole1();
	bool testDebugConsole2();


	inline void runTests()
	{
		printSuccessMessage(testInputHandler());
		printSuccessMessage(testDebugConsole1());
		printSuccessMessage(testDebugConsole2());
	}
}