#pragma once

#include "GameComponent.h"
#include <DirectXTK\Mouse.h>
#include <memory>

#include <windows.h>

namespace DirectX
{
	class Mouse;
}

namespace Library
{
	enum class MouseButtons
	{
		Left = 0,
		Right,
		Middle,
		X1,
		X2
	};

	enum class MouseModes
	{
		Absolute = 0,
		Relative
	};

	class MouseComponent final : public GameComponent
	{
		RTTI_DECLARATIONS_ABSTRACT(MouseComponent, GameComponent)

	public:
		static DirectX::Mouse* Mouse();
		
		explicit MouseComponent(Game& game, MouseModes mode = MouseModes::Relative);
		MouseComponent(const MouseComponent&) = delete;
		MouseComponent(MouseComponent&&) = default;
		MouseComponent& operator=(const MouseComponent&) = delete;
		MouseComponent& operator=(MouseComponent&&) = default;
		~MouseComponent() = default;

		const DirectX::Mouse::State& CurrentState() const;
		const DirectX::Mouse::State& LastState() const;

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		void SetWindow(HWND window);

		int X() const;
		int Y() const;
		int Wheel() const;

		bool IsButtonUp(MouseButtons button) const;
		bool IsButtonDown(MouseButtons button) const;
		bool WasButtonUp(MouseButtons button) const;
		bool WasButtonDown(MouseButtons button) const;
		bool WasButtonPressedThisFrame(MouseButtons button) const;
		bool WasButtonReleasedThisFrame(MouseButtons button) const;
		bool IsButtonHeldDown(MouseButtons Button) const;

		MouseModes Mode() const;
		void SetMode(MouseModes mode);

	private:
		bool GetButtonState(const DirectX::Mouse::State& state, MouseButtons button) const;

		static std::unique_ptr<DirectX::Mouse> sMouse;

		DirectX::Mouse::State mCurrentState;
		DirectX::Mouse::State mLastState;
	};
}