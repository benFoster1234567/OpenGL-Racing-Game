#include "core/input/InputHandler.h"
#include <iostream>

using namespace Engine::Core;

void Engine::Core::InputHandler::setKey(KeyCode key, bool pressed)
{
	inputState.currentFrameInputData.set(int(key), pressed);
}

void Engine::Core::InputHandler::updateKeyboard()
{
	inputState.previousFrameInputData = inputState.currentFrameInputData;
}

void Engine::Core::InputHandler::printDebugInfo()
{
	std::cout << "previous frame: " << inputState.previousFrameInputData << "\n";
	std::cout << "current frame: " << inputState.currentFrameInputData << "\n";
}