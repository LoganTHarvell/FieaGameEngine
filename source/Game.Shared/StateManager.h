#pragma once
#include <functional>
#include "InputManager.h"

namespace GameDesktop
{
	class StateManager final
	{
		using GameStateFunctors = std::function<void(InputManager&)>;

	public:
		enum class GameStates
		{
			Unknown = -1,

			Menu,
			TwoPlayer,
			ThreePlayer,
			FourPlayer,

			End
		};

		static GameStates GameState();
		static void ChangeState(GameStates state, InputManager& inputManager)
		{
			StateChangeMethods[static_cast<size_t>(state)](inputManager);
		}

	private:
		static void InitializeMenuState(InputManager& inputManager);
		static void InitializeTwoPlayerState(InputManager& inputManager);
		static void InitializeThreePlayerState(InputManager& inputManager);
		static void InitializeFourPlayerState(InputManager& inputManager);

	private:
		inline static GameStates mState;
		static const GameStateFunctors StateChangeMethods[static_cast<size_t>(GameStates::End)];
	};
}

