#include "pch.h"
#include "Player2InputReaction.h"
#include "Event.h"
#include "Player2InputMessage.h"

using namespace Library;

namespace GameDesktop
{
#pragma region Special Members
	Player2InputReaction::Player2InputReaction(const std::string& name) : PlayerInputReaction(TypeIdClass(), name)
	{
		Event<Player2InputMessage>::Subscribe(*this);
	}

	Player2InputReaction::~Player2InputReaction()
	{
		Event<Player2InputMessage>::Unsubscribe(*this);
	}

	Player2InputReaction::Player2InputReaction(const Player2InputReaction& rhs) : PlayerInputReaction(rhs)
	{
		Event<Player2InputMessage>::Subscribe(*this);
	}

	Player2InputReaction::Player2InputReaction(Player2InputReaction&& rhs) noexcept : PlayerInputReaction(rhs)
	{
		Event<Player2InputMessage>::Subscribe(*this);
	}
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	gsl::owner<Scope*> Player2InputReaction::Clone() const
	{
		return new Player2InputReaction(*this);
	}
#pragma endregion Virtual Copy Constructor
}