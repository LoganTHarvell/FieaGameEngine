#include "pch.h"
#include "Player1InputReaction.h"
#include "Event.h"
#include "Player1InputMessage.h"

using namespace Library;

namespace GameDesktop
{
#pragma region Special Members
	Player1InputReaction::Player1InputReaction(const std::string& name) : PlayerInputReaction(TypeIdClass(), name)
	{
		Event<Player1InputMessage>::Subscribe(*this);
	}

	Player1InputReaction::~Player1InputReaction()
	{
		Event<Player1InputMessage>::Unsubscribe(*this);
	}

	Player1InputReaction::Player1InputReaction(const Player1InputReaction& rhs) : PlayerInputReaction(rhs)
	{
		Event<Player1InputMessage>::Subscribe(*this);
	}

	Player1InputReaction::Player1InputReaction(Player1InputReaction&& rhs) noexcept : PlayerInputReaction(rhs)
	{
		Event<Player1InputMessage>::Subscribe(*this);
	}
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	gsl::owner<Scope*> Player1InputReaction::Clone() const
	{
		return new Player1InputReaction(*this);
	}
#pragma endregion Virtual Copy Constructor
}