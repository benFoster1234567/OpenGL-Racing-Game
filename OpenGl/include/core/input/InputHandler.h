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

		bool invokeControlHeld(Control c);
		bool invokeControlPressed(Control c);
		bool invokeControlReleased(Control c);

		bool invokeKeyHeld(KeyCode k);
		bool invokeKeyPressed(KeyCode k);
		bool invokeKeyReleased(KeyCode k);

		size_t subscribeControlPressed(std::function<void(Control)> func);
		size_t subscribeControlReleased(std::function<void(Control)> func);
		size_t subscribeControlHeld(std::function<void(Control)> func);
		
		size_t subscribeKeyPressed(std::function<void(KeyCode)> func);
		size_t subscribeKeyReleased(std::function<void(KeyCode)> func);
		size_t subscribeKeyHeld(std::function<void(KeyCode)> func);
		
	private:
		bool mapKeys(KeyboardMappings _km);//helper for constructor
		KeyboardMappings km{};

		std::map<KeyCode, Control> keyboardMap{};
		std::set<KeyCode> heldKeys{};
		std::set<Control> heldControlKey{};

		EventDispatcher<KeyCode> keyDownEventBus{};
		EventDispatcher<KeyCode> keyUpEventBus{};
		EventDispatcher<KeyCode> keyHeldEventBus{};

		EventDispatcher<Control> controlDownEventBus{};
		EventDispatcher<Control> controlUpEventBus{};
		EventDispatcher<Control> controlHeldEventBus{};

		bool checkHeld(KeyCode k) const
		{ 
			return heldKeys.contains(k); 
		}

		bool checkHeld(Control c) const
		{
			return heldControlKey.contains(c);
		}

	};


	/*
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

		typename std::map<KeyCode, EventDispatcher<>>::iterator
			getOrCreateEd(KeyCode key, std::map<KeyCode, EventDispatcher<>>& map);

		typename std::map<Control, EventDispatcher<>>::iterator
			getOrCreateEd(Control key, std::map<Control, EventDispatcher<>>& map);

		std::map<Control, EventDispatcher<>> m_onControlPressed;
		std::map<Control, EventDispatcher<>> m_onControlReleased;

		std::map<KeyCode, EventDispatcher<>> m_onKeyPressed;
		std::map<KeyCode, EventDispatcher<>> m_onKeyReleased;

	};
	*/
};