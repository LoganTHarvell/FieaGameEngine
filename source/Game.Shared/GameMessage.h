#pragma once

#include <string>

#include "StateManager.h"

namespace GameDesktop
{
	class GameMessage final
	{
	public:
		explicit GameMessage(std::string filename=std::string(), StateManager::GameStates playerCount=StateManager::GameStates::Unknown);

		const std::string Filename;
		const StateManager::GameStates GameState;
	};
}
