#pragma once

#include "GameTime.h"

namespace Library
{
	/// <summary>
	/// Handle for data that makes up the state of a World.
	/// </summary>
	struct WorldState final
	{
	public:
		const GameTime& GetGameTime() const { return mGameTime; }
		void SetGameTime(const GameTime& gameTime) { mGameTime = gameTime; }

	public:
		class World* World{ nullptr };
		class Sector* Sector{ nullptr };
		class Entity* Entity{ nullptr };
		class Action* Action{ nullptr };
	
	private:
		GameTime mGameTime;
	};
}

