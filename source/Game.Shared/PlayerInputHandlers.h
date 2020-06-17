#pragma once

#include <functional>
#include <GLFW/glfw3.h>
#include <chrono>
#include <memory>
#include <windows.h>
#include "EventQueue.h"
#include "World.h"
#include "Event.h"
#include "Player1InputMessage.h"
#include "Player2InputMessage.h"
#include "Player3InputMessage.h"
#include "Player4InputMessage.h"

namespace GameDesktop
{

	struct PlayerInputHandlers
	{
		struct Debouncers
		{
			bool aDebounce;
			bool bDebounce;
			bool yDebounce;
			bool xDebounce;
			bool moveDebounce;
		};

		inline static const float AxisClamp = 0.7f;

		static void Player1Input(int key, int scancode, int action, int mods, Library::World& world);
		static void Player2Input(Library::World& world);
		static void Player3Input(Library::World& world);
		static void Player4Input(Library::World& world);

		template<typename T>
		static void ControllerInput(Library::World& world, int controllerId, Debouncers& debouncer);

		inline static int currentKey;
		inline static Debouncers p2Debouncers;
		inline static Debouncers p3Debouncers;
		inline static Debouncers p4Debouncers;
	};
}

#include "PlayerInputHandler.inl"