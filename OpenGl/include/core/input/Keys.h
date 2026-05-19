#pragma once
#include <cstdint>
namespace Engine::Core 
{
	enum class KeyCode : uint8_t
	{
		None = 0,
		A = 1, B = 2, C = 3, D = 4, E = 5, F = 6,
		G = 7, H = 8, I = 9, J = 10, K = 11, L = 12,
		M = 13, N = 14, O = 15, P = 16, Q = 17, R = 18,
		S = 19, T = 20, U = 21, V = 22, W = 23, X = 24,
		Y = 25, Z = 26,

		_0 = 27, _1 = 28, _2 = 29, _3 = 30, _4 = 31,
		_5 = 32, _6 = 33, _7 = 34, _8 = 35, _9 = 36,

		Up = 37,
		Down = 38,
		Left = 39,
		Right = 40,
		Tab = 41,
		Space = 42,
		Enter = 43,
		Any = 44

	};

	enum class KeyMod
	{
		NONE = 0,
		SHIFT = 1,
		ALT = 1 << 1,
		CTRL = 1 << 2,
	};
}