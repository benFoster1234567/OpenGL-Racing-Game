#pragma once

#include <map>
#include <set>
#include <vector>

#include <glm/mat4x4.hpp>

#include "Keys.h"
#include "core/events/EventDispatcher.h"
#include <bitset>


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

	struct KeyboardInputResource
	{
		std::bitset<512> currentFrameInputData{};
		std::bitset<512> previousFrameInputData{};

		glm::vec2 mousePos{0,0};
		glm::vec2 lastMousePos{0,0};
		glm::vec2 mouseDelta{0,0};
		
		//add mouseButtons
		void updateMouseState()
		{
			lastMousePos = mousePos;
			mouseDelta = { 0.0,0.0 };
		}

		void updateMouseState(glm::vec2 currentMouse/* , currentMouseButtons */);
	};

	class KeyboardBridge
	{
	private:

	public:
		
		KeyboardInputResource inputState{};
		void setKey(KeyCode k, bool pressed);

		void updateKeyboard();
		void updateMousePosition(glm::vec2 pos) { inputState.updateMouseState(pos); }

		void printDebugInfo();

		bool keyPressed(int key) const
		{
			//if (key >= 512) throw std::runtime_error("invalid key!");
			return inputState.currentFrameInputData.test(key) && !inputState.previousFrameInputData.test(key);
		}

		bool keyReleased(int key) const
		{
			//if (key >= 512) throw std::runtime_error("invalid key!");
			return inputState.currentFrameInputData.test(key) == false && inputState.previousFrameInputData.test(key) == true;
		}

		bool keyHeld(int key) const
		{
			//if (key >= 512) throw std::runtime_error("invalid key!");
			return inputState.currentFrameInputData.test(key) && inputState.previousFrameInputData.test(key);
		}

	};

};