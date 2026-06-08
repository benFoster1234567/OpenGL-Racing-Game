#include "core/input/InputHandler.h"

using namespace Engine::Core;

InputHandler::InputHandler(KeyboardMappings keymaps)
{

	mapKeys(keymaps);
}

void Engine::Core::InputHandler::invokeKeyAndControl(KeyCode k, KeyAction a)
{
	Control c = Control::None;
	auto it = keyboardMap.find(k);
	if (it != keyboardMap.end())
	{
		c = it->second;
	}
	invokeControl(c, a);
	invokeKey(k, a);	
}

void Engine::Core::InputHandler::invokeControl(Control c, KeyAction a)
{
	if (c == Control::None) return;
	controlBus.invoke(c, a);
}

void Engine::Core::InputHandler::invokeKey(KeyCode k, KeyAction a)
{
	keyBus.invoke(k, a);
}

size_t Engine::Core::InputHandler::subscribeKey(std::function<void(KeyCode, KeyAction)> func)
{
	return keyBus.subscribe(func);
}

size_t Engine::Core::InputHandler::subscribeControl(std::function<void(Control, KeyAction)> func)
{
	return controlBus.subscribe(func);
}

bool InputHandler::mapKeys(KeyboardMappings _km)
{
	//need km for yaml support?
	keyboardMap.insert({ _km.lookup, Control::LookUp });
	keyboardMap.insert({ _km.lookdown, Control::LookDown });
	keyboardMap.insert({ _km.lookleft, Control::LookLeft });
	keyboardMap.insert({ _km.lookright, Control::LookRight });
	keyboardMap.insert({ _km.turnleft, Control::TurnLeft });
	keyboardMap.insert({ _km.turnright, Control::TurnRight });
	keyboardMap.insert({ _km.acceleratepedal, Control::Accelerate });
	keyboardMap.insert({ _km.breakpedal, Control::BreakPedal });
	keyboardMap.insert({ _km.ebrake, Control::EBrake });
	km = _km;
	return true;
}


