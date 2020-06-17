#include "pch.h"
#include "StateManager.h"
#include "PlayerInputHandlers.h"
#include "MenuInputHandler.h"

namespace GameDesktop
{
	const StateManager::GameStateFunctors StateManager::StateChangeMethods[] = {
		&StateManager::InitializeMenuState,
		&StateManager::InitializeTwoPlayerState,
		&StateManager::InitializeThreePlayerState,
		&StateManager::InitializeFourPlayerState
	};

	StateManager::GameStates StateManager::GameState()
	{
		return mState;
	}

	void StateManager::InitializeMenuState([[maybe_unused]] InputManager& inputManager)
	{
		inputManager.ClearInputHandlers();
		inputManager.AddKeyboardHandler(MenuInput);
		mState = GameStates::Menu;
	}

	void StateManager::InitializeTwoPlayerState(InputManager& inputManager)
	{
		inputManager.ClearInputHandlers();
		inputManager.AddKeyboardHandler(PlayerInputHandlers::Player1Input);
		inputManager.AddGamepadHandler(PlayerInputHandlers::Player2Input);
		mState = GameStates::TwoPlayer;
	}

	void StateManager::InitializeThreePlayerState(InputManager& inputManager)
	{
		inputManager.ClearInputHandlers();
		inputManager.AddKeyboardHandler(PlayerInputHandlers::Player1Input);
		inputManager.AddGamepadHandler(PlayerInputHandlers::Player2Input);
		inputManager.AddGamepadHandler(PlayerInputHandlers::Player3Input);
		mState = GameStates::ThreePlayer;
	}

	void StateManager::InitializeFourPlayerState(InputManager& inputManager)
	{
		inputManager.ClearInputHandlers();
		inputManager.AddKeyboardHandler(PlayerInputHandlers::Player1Input);
		inputManager.AddGamepadHandler(PlayerInputHandlers::Player2Input);
		inputManager.AddGamepadHandler(PlayerInputHandlers::Player3Input);
		inputManager.AddGamepadHandler(PlayerInputHandlers::Player4Input);
		mState = GameStates::FourPlayer;
	}
}