#pragma once

#include "GameTime.h"

namespace Library
{
	/// <summary>
	/// Handle for data that makes up the current state of a World during processing.
	/// </summary>
	struct WorldState final
	{
	public:
		/// <summary>
		/// Gets the GameTime associated with the WorldState.
		/// </summary>
		/// <returns>Current GameTime.</returns>
		const GameTime& GetGameTime() const { return mGameTime; }

		/// <summary>
		/// Sets the GameTime associated with the WorldState.
		/// </summary>
		/// <param name="gameTime">GameTime to set the WorldState instance.</param>
		void SetGameTime(const GameTime& gameTime) { mGameTime = gameTime; }

	public:
		/// <summary>
		/// Handle to the current World.
		/// </summary>
		class World* World{ nullptr };

		/// <summary>
		/// Handle to the current Sector.
		/// </summary>
		class Sector* Sector{ nullptr };

		/// <summary>
		/// Handle to the current Sector.
		/// </summary>
		class Entity* Entity{ nullptr };

		/// <summary>
		/// Handle to the current Action.
		/// </summary>
		class Action* Action{ nullptr };
	
	private:
		/// <summary>
		/// GameTime associated with the current frame being processed.
		/// </summary>
		GameTime mGameTime;
	};
}

