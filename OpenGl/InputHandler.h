#pragma once

#include <map>
#include <set>
#include <vector>

#include <glm/mat4x4.hpp>

#include "Keys.h"
#include "EventDispatcher.h"





enum class Control 
{
	LookUp, 
	LookDown,
	LookLeft,
	LookRight,
	TurnLeft,
	TurnRight,
	Accelerate,
	BreakPedal,
	EBrake,
};


struct KeyboardMappings
{
	KeyboardMappings() = default;
	KeyCode lookup = KeyCode::Up;
	KeyCode lookdown = KeyCode::Down;
	KeyCode lookleft = KeyCode::Left;
	KeyCode lookright = KeyCode::Right;
	KeyCode turnleft = KeyCode::A;
	KeyCode turnright = KeyCode::D;
	KeyCode acceleratepedal = KeyCode::A;
	KeyCode breakpedal = KeyCode::S;
	KeyCode ebrake = KeyCode::Space;
};


class InputHandler
{
public:
	InputHandler() { mapKeys({}); }
	~InputHandler() = default;

	InputHandler(KeyboardMappings keymaps);
	bool invokeKeyPressed(KeyCode key);
	bool invokeKeyReleased(KeyCode key);

	bool invokeControlPressed(Control c);
	bool invokeControlReleased(Control c);

	bool bindKeyPressed(KeyCode key, EventDispatcher<>::CallBackFunc);
	bool bindKeyReleased(KeyCode key, EventDispatcher<>::CallBackFunc);
	bool bindControlPressed(Control c, EventDispatcher<>::CallBackFunc);
	bool bindControlReleased(Control c, EventDispatcher<>::CallBackFunc);


private:
	bool mapKeys(KeyboardMappings km);//helper for constructor


	std::map<Control, KeyCode> keyboardMap;

	KeyCode lastKeyPressed{ KeyCode::None };
	KeyCode lastKeyReleased{ KeyCode::None };

	typename std::map<KeyCode, EventDispatcher<>>::iterator getOrCreateEd(KeyCode key, std::map<KeyCode, EventDispatcher<>>& map);
	typename std::map<Control, EventDispatcher<>>::iterator getOrCreateEd(Control key, std::map<Control, EventDispatcher<>>& map);

	std::map<Control, EventDispatcher<>> m_onControlPressed;
	std::map<Control, EventDispatcher<>> m_onControlReleased;

	std::map<KeyCode, EventDispatcher<>> m_onKeyPressed;
	std::map<KeyCode, EventDispatcher<>> m_onKeyReleased;

};