#include "pch.h"
#include "Player3InputReaction.h"
#include "Event.h"
#include "Player3InputMessage.h"

using namespace Library;

namespace GameDesktop
{
#pragma region Special Members
	Player3InputReaction::Player3InputReaction(const std::string& name) : PlayerInputReaction(TypeIdClass(), name)
	{
		Event<Player3InputMessage>::Subscribe(*this);
	}

	Player3InputReaction::~Player3InputReaction()
	{
		Event<Player3InputMessage>::Unsubscribe(*this);
	}

	Player3InputReaction::Player3InputReaction(const Player3InputReaction& rhs) : PlayerInputReaction(rhs)
	{
		Event<Player3InputMessage>::Subscribe(*this);
	}

	Player3InputReaction::Player3InputReaction(Player3InputReaction&& rhs) noexcept : PlayerInputReaction(rhs)
	{
		Event<Player3InputMessage>::Subscribe(*this);
	}
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	gsl::owner<Scope*> Player3InputReaction::Clone() const
	{
		return new Player3InputReaction(*this);
	}
#pragma endregion Virtual Copy Constructor
}