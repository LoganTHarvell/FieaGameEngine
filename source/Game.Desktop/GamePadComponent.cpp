#include "pch.h"
#include "GamePadComponent.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	unique_ptr<GamePad> GamePadComponent::sGamePad(new DirectX::GamePad);

	GamePad* GamePadComponent::GamePad()
	{
		return sGamePad.get();
	}

	GamePadComponent::GamePadComponent(Game& game, int player) :
		GameComponent(game), mPlayer(player)
	{
	}

	int GamePadComponent::Player() const
	{
		return mPlayer;
	}

	const GamePad::State& GamePadComponent::CurrentState() const
	{
		return mCurrentState;
	}

	const GamePad::State& GamePadComponent::LastState() const
	{
		return mLastState;
	}

	void GamePadComponent::Initialize()
	{
		mCurrentState = sGamePad->GetState(mPlayer);
		mLastState = mCurrentState;
	}

	void GamePadComponent::Update(const GameTime&)
	{
		mLastState = mCurrentState;
		mCurrentState = sGamePad->GetState(mPlayer);
	}

	bool GamePadComponent::IsButtonUp(const GamePadButtons button) const
	{
		return GetButtonState(mCurrentState, button) == false;
	}

	bool GamePadComponent::IsButtonDown(const GamePadButtons button) const
	{
		return GetButtonState(mCurrentState, button);
	}

	bool GamePadComponent::WasButtonUp(const GamePadButtons button) const
	{
		return GetButtonState(mLastState, button) == false;
	}

	bool GamePadComponent::WasButtonDown(const GamePadButtons button) const
	{
		return GetButtonState(mLastState, button);
	}

	bool GamePadComponent::WasButtonPressedThisFrame(const GamePadButtons button) const
	{
		return (IsButtonDown(button) && WasButtonUp(button));
	}

	bool GamePadComponent::WasButtonReleasedThisFrame(const GamePadButtons button) const
	{
		return (IsButtonUp(button) && WasButtonDown(button));
	}

	bool GamePadComponent::IsButtonHeldDown(const GamePadButtons button) const
	{
		return (IsButtonDown(button) && WasButtonDown(button));
	}

	bool GamePadComponent::GetButtonState(const GamePad::State& state, const GamePadButtons button)
	{
		switch (button)
		{
			case GamePadButtons::A:
				return state.buttons.a;

			case GamePadButtons::B:
				return state.buttons.b;

			case GamePadButtons::X:
				return state.buttons.x;

			case GamePadButtons::Y:
				return state.buttons.y;

			case GamePadButtons::LeftStick:
				return state.buttons.leftStick;

			case GamePadButtons::RightStick:
				return state.buttons.rightStick;

			case GamePadButtons::LeftShoulder:
				return state.buttons.leftShoulder;

			case GamePadButtons::RightShoulder:
				return state.buttons.rightShoulder;

			case GamePadButtons::Back:
				return state.buttons.back;

			case GamePadButtons::Start:
				return state.buttons.start;

			case GamePadButtons::DPadUp:
				return state.dpad.up;

			case GamePadButtons::DPadDown:
				return state.dpad.down;

			case GamePadButtons::DPadLeft:
				return state.dpad.left;

			case GamePadButtons::DPadRight:
				return state.dpad.right;

			default:
				throw exception("Invalid GamePadButtons.");
		}
	}
}