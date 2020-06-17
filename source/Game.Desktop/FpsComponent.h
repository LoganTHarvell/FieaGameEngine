#pragma once

#include "DrawableGameComponent.h"
#include <DirectXMath.h>
#include <chrono>

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Library
{
	class FpsComponent final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS_ABSTRACT(FpsComponent, DrawableGameComponent)

	public:
		FpsComponent(Game& game);
		FpsComponent(const FpsComponent&) = delete;
		FpsComponent(FpsComponent&&) = default;
		FpsComponent& operator=(const FpsComponent&) = delete;
		FpsComponent& operator=(FpsComponent&&) = default;
		~FpsComponent() = default;

		DirectX::XMFLOAT2& TextPosition();
		int FrameCount() const;	
		int FrameRate() const;

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> mSpriteFont;
		DirectX::XMFLOAT2 mTextPosition{ 0.0f, 20.0f };

		int mFrameCount{ 0 };
		int mFrameRate{ 0 };
		std::chrono::milliseconds mLastTotalGameTime;
	};
}