#pragma once
#include "EventDispatcher.h"
#include "InputHandler.h"
#include <iostream>
#include <string>


namespace Tester
{
	void printSuccessMessage(bool success, const std::string& successMsg = "test passed ! "
		, const std::string& failureMsg = "test failed !", std::ostream& out = std::cout);

	bool testInputHandler();

	inline void runTests()
	{
		printSuccessMessage(testInputHandler());
	}
}