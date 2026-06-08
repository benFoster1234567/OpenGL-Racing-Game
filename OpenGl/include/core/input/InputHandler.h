#pragma once

#include <map>
#include <set>
#include <vector>

#include <glm/mat4x4.hpp>

#include "Keys.h"
#include "core/events/EventDispatcher.h"


namespace Engine::Core
{


	enum class Control
	{
		None,
		LookUp,
		LookDown,
		LookLeft,
		LookRight,
		TurnLeft,
		TurnRight,
		Accelerate,
		BreakPedal,
		EBrake,
		Console
	};

	enum class KeyAction
	{
		Down,
		Up,
		Held
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
		KeyCode Console = KeyCode::BackTick;
	};

	class InputHandler
	{
	public:
		InputHandler() { mapKeys({}); }
		InputHandler(KeyboardMappings keymaps);
		InputHandler(const InputHandler&) = default;
		InputHandler(InputHandler&&) = default;
		~InputHandler() = default;

		void invokeKeyAndControl(KeyCode k, KeyAction a);

		void invokeControl(Control c, KeyAction a);
		void invokeKey(KeyCode k, KeyAction a);
		
		size_t subscribeKey(std::function<void(KeyCode, KeyAction)> func);
		size_t subscribeControl(std::function<void(Control, KeyAction)> func);


	private:
		bool mapKeys(KeyboardMappings _km);//helper for constructor
		KeyboardMappings km{};

		std::map<KeyCode, Control> keyboardMap{};
		std::set<KeyCode> heldKeys{};
		std::set<Control> heldControlKey{};

		EventDispatcher<KeyCode, KeyAction> keyBus{};
		EventDispatcher<Control, KeyAction> controlBus{};

		bool checkHeld(KeyCode k) const
		{
			return heldKeys.contains(k);
		}

		bool checkHeld(Control c) const
		{
			return heldControlKey.contains(c);
		}

	};

};