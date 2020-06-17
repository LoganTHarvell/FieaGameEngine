#include "pch.h"

#include "GameMessage.h"

namespace GameDesktop
{
	GameMessage::GameMessage(std::string filename, StateManager::GameStates newState) :
		Filename(std::move(filename)), GameState(newState)
	{
	}
}