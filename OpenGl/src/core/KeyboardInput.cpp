#include "core/input/KeyboardInput.h"
#include <iostream>

using namespace Engine::Core;

void Engine::Core::KeyboardBridge::setKey(KeyCode key, bool pressed)
{
	inputState.currentFrameInputData.set(int(key), pressed);
}

void Engine::Core::KeyboardBridge::updateKeyboard()
{
	inputState.previousFrameInputData = inputState.currentFrameInputData;
}

void Engine::Core::KeyboardBridge::printDebugInfo()
{
	std::cout << "previous frame: " << inputState.previousFrameInputData << "\n";
	std::cout << "current frame: " << inputState.currentFrameInputData << "\n";
}

void Engine::Core::KeyboardInputResource::updateMouseState(glm::vec2 currentMouse)
{
}
