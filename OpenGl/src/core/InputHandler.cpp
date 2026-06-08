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

	switch (a)
	{
	case KeyAction::Down:
		invokeKeyPressed(k);
		invokeControlPressed(c);
		break;
	case KeyAction::Up:
		invokeKeyReleased(k);
		invokeControlReleased(c);
		break;
	case KeyAction::Held:
		invokeKeyHeld(k);
		invokeControlHeld(c);
		break;
	}
}

bool Engine::Core::InputHandler::invokeControlHeld(Control c)
{
	if (c == Control::None) return false;
	if (heldControlKey.contains(c))
	{
		controlHeldEventBus.invoke(c);
		return true;
	}
	return false;
}

bool Engine::Core::InputHandler::invokeControlPressed(Control c)
{
	if (c == Control::None) return false;
	if (heldControlKey.contains(c)) return false;
	controlDownEventBus.invoke(c);
	heldControlKey.insert(c);
	return true;
}

bool Engine::Core::InputHandler::invokeControlReleased(Control c)
{
	if (c == Control::None) return false;
	if (heldControlKey.contains(c)) heldControlKey.erase(c);
	controlUpEventBus.invoke(c);
	return true;
}

bool Engine::Core::InputHandler::invokeKeyHeld(KeyCode k)
{
	if (heldKeys.contains(k))
	{
		keyHeldEventBus.invoke(k);
		return true;
	}
	return false;
}

bool Engine::Core::InputHandler::invokeKeyPressed(KeyCode k)
{
	if (heldKeys.contains(k)) return false;
	keyDownEventBus.invoke(k);
	heldKeys.insert(k);
	return true;
}

bool Engine::Core::InputHandler::invokeKeyReleased(KeyCode k)
{
	if (heldKeys.contains(k)) heldKeys.erase(k);
	keyUpEventBus.invoke(k);
	return true;
}

size_t Engine::Core::InputHandler::subscribeControlPressed(std::function<void(Control)> func)
{
	return controlDownEventBus.subscribe(func);
}

size_t Engine::Core::InputHandler::subscribeControlReleased(std::function<void(Control)> func)
{
	return controlUpEventBus.subscribe(func);
}

size_t Engine::Core::InputHandler::subscribeControlHeld(std::function<void(Control)> func)
{
	return controlHeldEventBus.subscribe(func);
}

size_t Engine::Core::InputHandler::subscribeKeyPressed(std::function<void(KeyCode)> func)
{
	return keyDownEventBus.subscribe(func);
}

size_t Engine::Core::InputHandler::subscribeKeyReleased(std::function<void(KeyCode)> func)
{
	return keyHeldEventBus.subscribe(func);
}

size_t Engine::Core::InputHandler::subscribeKeyHeld(std::function<void(KeyCode)> func)
{
	return keyHeldEventBus.subscribe(func);
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


/*
InputHandler::InputHandler(KeyboardMappings keymaps)
{
	
	mapKeys(keymaps);
}

bool InputHandler::invokeKeyPressed(KeyCode key)
{
	auto it = m_onKeyPressed.find(key);
	if (it == m_onKeyPressed.end()) return false;
	(*it).second.invoke();
	return true;
}

bool InputHandler::invokeKeyReleased(KeyCode key)
{
	auto it = m_onKeyReleased.find(key);
	if (it == m_onKeyReleased.end()) return false;
	(*it).second.invoke();
	return true;
}

bool InputHandler::invokeControlPressed(Control c)
{
	auto it = m_onControlPressed.find(c);
	if (it == m_onControlPressed.end()) return false;
	(*it).second.invoke();
	return true;
}

bool InputHandler::invokeControlReleased(Control c)
{
	auto it = m_onControlReleased.find(c);
	if (it == m_onControlReleased.end()) return false;
	(*it).second.invoke();
	return true;
}

bool InputHandler::bindKeyPressed(KeyCode key, EventDispatcher<>::CallBackFunc func)
{
	auto it = getOrCreateEd(key, m_onKeyPressed);
	(*it).second.subscribe(func);
	return true;
}

bool InputHandler::bindKeyReleased(KeyCode key, EventDispatcher<>::CallBackFunc func)
{
	auto it = getOrCreateEd(key, m_onKeyReleased);
	(*it).second.subscribe(func);
	return true;
}

bool InputHandler::bindControlPressed(Control c, EventDispatcher<>::CallBackFunc func)
{
	auto it = getOrCreateEd(c, m_onControlPressed);
	(*it).second.subscribe(func);
	return true;
}

bool InputHandler::bindControlReleased(Control c, EventDispatcher<>::CallBackFunc func)
{
	auto it = getOrCreateEd(c, m_onControlReleased);
	(*it).second.subscribe(func);
	return true;
}

bool InputHandler::mapKeys(KeyboardMappings km)
{
	//need km for yaml support?
	keyboardMap.insert({ Control::LookUp, km.lookup });
	keyboardMap.insert({ Control::LookDown, km.lookdown});
	keyboardMap.insert({ Control::LookLeft, km.lookleft});
	keyboardMap.insert({ Control::LookRight, km.lookright});
	keyboardMap.insert({ Control::TurnLeft, km.turnleft});
	keyboardMap.insert({ Control::TurnRight, km.turnright});
	keyboardMap.insert({ Control::Accelerate, km.acceleratepedal});
	keyboardMap.insert({ Control::BreakPedal, km.breakpedal});
	keyboardMap.insert({ Control::EBrake, km.ebrake});
	return false;
}

typename std::map<KeyCode, EventDispatcher<>>::iterator 
InputHandler::getOrCreateEd(KeyCode key, std::map<KeyCode, EventDispatcher<>>& map)
{
	auto it = map.find(key);
	if (it == map.end()) map.emplace(key, EventDispatcher{});
	it = map.find(key);
	return it;
}

typename std::map<Control, EventDispatcher<>>::iterator 
InputHandler::getOrCreateEd(Control key, std::map<Control, EventDispatcher<>>& map)
{
	auto it = map.find(key);
	if (it == map.end()) map.emplace(key, EventDispatcher{});
	it = map.find(key);
	return it;
}
*/