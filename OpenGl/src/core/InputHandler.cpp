#include "core/input/InputHandler.h"

using namespace Engine::Core;

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
