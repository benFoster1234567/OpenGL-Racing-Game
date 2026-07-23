#include "core/input/KeyboardInput.h"
#include <iostream>

using namespace Engine::Core;

void Engine::Core::InputBridge::setKey(KeyCode key, bool pressed)
{
	inputState.currentFrameInputData.set(int(key), pressed);
}

void Engine::Core::InputBridge::updateKeyboard()
{
	inputState.previousFrameInputData = inputState.currentFrameInputData;
}

void Engine::Core::InputBridge::printDebugInfo()
{
	std::cout << "previous frame: " << inputState.previousFrameInputData << "\n";
	std::cout << "current frame: " << inputState.currentFrameInputData << "\n";
}

void Engine::Core::MouseInputResource::zeroMouseDelta()
{
	lastMousePos = { 0.0,0.0 };
	mouseDelta = { 0.0,0.0 };
}

void Engine::Core::MouseInputResource::updateMouseState(glm::vec2 currentMouse)
{
	mouseDelta = currentMouse - mousePos;
	lastMousePos = mousePos;
	mousePos   = currentMouse;
}
