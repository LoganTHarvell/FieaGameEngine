#include "pch.h"
#include "FpsComponent.h"
#include "Game.h"

using namespace std;
using namespace std::literals;
using namespace DirectX;

namespace Library
{
	FpsComponent::FpsComponent(Game& game) :
		DrawableGameComponent(game)		
	{
	}

	XMFLOAT2& FpsComponent::TextPosition()
	{
		return mTextPosition;
	}

	int FpsComponent::FrameCount() const
	{
		return mFrameCount;
	}

	int FpsComponent::FrameRate() const
	{
		return mFrameRate;
	}

	void FpsComponent::Initialize()
	{
		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");
	}

	void FpsComponent::Update(const GameTime& gameTime)
	{
		if ((gameTime.TotalGameTime() - mLastTotalGameTime) >= 1s)
		{
			mLastTotalGameTime = gameTime.TotalGameTime();
			mFrameRate = mFrameCount;
			mFrameCount = 0;
		}

		++mFrameCount;
	}

	void FpsComponent::Draw(const GameTime& gameTime)
	{
		mSpriteBatch->Begin();

		wostringstream fpsLabel;
		fpsLabel << setprecision(4) << L"Frame Rate: " << mFrameRate << L"    Total Elapsed Time: " << gameTime.TotalGameTimeSeconds().count();
		mSpriteFont->DrawString(mSpriteBatch.get(), fpsLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
	}
}