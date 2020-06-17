#pragma once

#include <windows.h>
#include <functional>

#include "FirstPersonCamera.h"
#include "Game.h"

namespace Library
{
	class KeyboardComponent;
	class MouseComponent;
	class GamePadComponent;
	class FpsComponent;
	class Grid;
}

namespace Demo
{
	class SolarSystemDemo;

	class RenderingGame final : public Library::Game
	{
	public:
		RenderingGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		virtual void Shutdown() override;

		void Exit();

	private:
		inline static const DirectX::XMVECTORF32 BackgroundColor{ DirectX::Colors::CornflowerBlue };

		void UpdateAmbientLightIntensity(const Library::GameTime& gameTime);
		void UpdatePointLight(const Library::GameTime& gameTime);
		void UpdateSpecularLight(const Library::GameTime& gameTime);

		std::shared_ptr<Library::FirstPersonCamera> mCamera;
		std::shared_ptr<Library::KeyboardComponent> mKeyboard;
		std::shared_ptr<Library::MouseComponent> mMouse;
		std::shared_ptr<Library::GamePadComponent> mGamePad;
		std::shared_ptr<Library::FpsComponent> mFpsComponent;
		std::shared_ptr<Library::Grid> mGrid;
		std::shared_ptr<SolarSystemDemo> mSolarSystemDemo;
		float mAmbientLightIntensity{ 0.0f };
		float mPointLightIntensity{ 0.0f };
		float mSpecularIntensity{ 0.0f };
		float mSpecularPower{ 128.0f };
	};
}