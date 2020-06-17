#pragma once

#include "World.h"
#include "WorldState.h"

#include "GameTime.h"

#include "PlayerInputMessage.h"

namespace GameDesktop
{
	using namespace std::string_literals;
	using namespace Library;

	class AIManager final
	{
	public:
		enum class EInitState : int
		{
			Unknown = -1,
			NonePlayer = 0,
			OnePlayer = 1,
			TwoPlayer = 2,
			ThreePlayer = 3,
			FourPlayer = 4,
			End
		};

		AIManager(EInitState state = EInitState::Unknown, GameTime gameTime = GameTime());

		void PakRandomMoveInput(PlayerInputMessage* message);
		void PakBomb(PlayerInputMessage* message);

		void UpdatePlayer1AI(WorldState& worldState);
		void UpdatePlayer2AI(WorldState& worldState);
		void UpdatePlayer3AI(WorldState& worldState);
		void UpdatePlayer4AI(WorldState& worldState);

		void UpdateAI(WorldState& worldState);

		void ResetAITimeDelay(GameTime gameTime);

		~AIManager() = default;
		AIManager(const AIManager& rhs) = default;
		AIManager(AIManager&& rhs) = default;
		AIManager& operator=(const AIManager& rhs) = default;
		AIManager& operator=(AIManager&& rhs) = default;

		private:
			EInitState mPlayersState;

			int mBombSpawnRate = 10;
			std::chrono::high_resolution_clock::time_point mStartTime;
			std::chrono::milliseconds mInitialDelay = std::chrono::seconds(5);
	};
}