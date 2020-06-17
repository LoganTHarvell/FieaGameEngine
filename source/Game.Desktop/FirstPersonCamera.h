#pragma once

#include "PerspectiveCamera.h"
#include "GamePadComponent.h"
#include <functional>
#include <vector>

namespace Library
{	
	class KeyboardComponent;
	class MouseComponent;

    class FirstPersonCamera : public PerspectiveCamera
    {
		RTTI_DECLARATIONS_ABSTRACT(FirstPersonCamera, PerspectiveCamera)

    public:
        explicit FirstPersonCamera(Game& game, float fieldOfView = DefaultFieldOfView, float aspectRatio = DefaultAspectRatio, float nearPlaneDistance = DefaultNearPlaneDistance, float farPlaneDistance = DefaultFarPlaneDistance);
		FirstPersonCamera(const FirstPersonCamera&) = default;
		FirstPersonCamera& operator=(const FirstPersonCamera&) = default;
		FirstPersonCamera(FirstPersonCamera&&) = default;
		FirstPersonCamera& operator=(FirstPersonCamera&&) = default;
		virtual ~FirstPersonCamera() = default;

		GamePadComponent* GetGamePad() const;
		void SetGamePad(GamePadComponent* gamePad);

		KeyboardComponent* GetKeyboard() const;
		void SetKeyboard(KeyboardComponent* keyboard);

		MouseComponent* GetMouse() const;
		void SetMouse(MouseComponent* mouse);

		float& MouseSensitivity();
        float& RotationRate();
        float& MovementRate();
        
		const std::vector<std::function<void()>>& PositionUpdatedCallbacks() const;
		void AddPositionUpdatedCallback(std::function<void()> callback);

		virtual void SetPosition(float x, float y, float z) override;
		virtual void SetPosition(DirectX::FXMVECTOR position) override;
		virtual void SetPosition(const DirectX::XMFLOAT3& position) override;

		virtual void Initialize() override;
        virtual void Update(const GameTime& gameTime) override;

		inline static const float DefaultMouseSensitivity{ 0.1f };
		inline static const float DefaultRotationRate{ DirectX::XMConvertToRadians(100.0f) };
		inline static const float DefaultMovementRate{ 100.0f };

	private:
		void UpdatePosition(const DirectX::XMFLOAT2& movementAmount, const DirectX::XMFLOAT2& rotationAmount, const GameTime& gameTime);
		void InvokePositionUpdatedCallbacks();
		
		inline bool IsGamePadConnected(DirectX::GamePad::State& gamePadState)
		{
			if (mGamePad != nullptr)
			{
				gamePadState = mGamePad->CurrentState();
				return gamePadState.IsConnected();
			}

			return false;
		}

    protected:
		GamePadComponent* mGamePad{ nullptr };
		KeyboardComponent* mKeyboard{ nullptr };
		MouseComponent* mMouse{ nullptr };
		float mMouseSensitivity{ DefaultMouseSensitivity };
		float mRotationRate{ DefaultRotationRate };
        float mMovementRate{ DefaultMovementRate };
		std::vector<std::function<void()>> mPositionUpdatedCallbacks;
    };
}