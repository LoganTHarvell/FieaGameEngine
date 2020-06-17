#include "pch.h"
#include "Player4InputReaction.h"
#include "Event.h"
#include "Player4InputMessage.h"

using namespace Library;

namespace GameDesktop
{
#pragma region Special Members
	Player4InputReaction::Player4InputReaction(const std::string& name) : PlayerInputReaction(TypeIdClass(), name)
	{
		Event<Player4InputMessage>::Subscribe(*this);
	}

	Player4InputReaction::~Player4InputReaction()
	{
		Event<Player4InputMessage>::Unsubscribe(*this);
	}

	Player4InputReaction::Player4InputReaction(const Player4InputReaction& rhs) : PlayerInputReaction(rhs)
	{
		Event<Player4InputMessage>::Subscribe(*this);
	}

	Player4InputReaction::Player4InputReaction(Player4InputReaction&& rhs) noexcept : PlayerInputReaction(rhs)
	{
		Event<Player4InputMessage>::Subscribe(*this);
	}
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	gsl::owner<Scope*> Player4InputReaction::Clone() const
	{
		return new Player4InputReaction(*this);
	}
#pragma endregion Virtual Copy Constructor
}