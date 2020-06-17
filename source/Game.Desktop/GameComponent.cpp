#include "pch.h"
#include "GameComponent.h"

namespace Library
{
	GameComponent::GameComponent(Game& game) :
		mGame(&game)
	{
	}

	Game* GameComponent::GetGame()
	{
		return mGame;
	}

	void GameComponent::SetGame(Game& game)
	{
		mGame = gsl::not_null<Game*>(&game);
	}

	bool GameComponent::Enabled() const
	{
		return mEnabled;
	}

	void GameComponent::SetEnabled(bool enabled)
	{
		mEnabled = enabled;
	}

	void GameComponent::Initialize()
	{
	}

	void GameComponent::Shutdown()
	{
	}

	void GameComponent::Update(const GameTime&)
	{		
	}
}