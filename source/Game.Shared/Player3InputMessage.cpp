#include "pch.h"
#include "Player3InputMessage.h"

using namespace Library;

namespace GameDesktop
{
	Player3InputMessage::Player3InputMessage(MessageType type) : PlayerInputMessage(type) {}

	gsl::owner<Scope*> Player3InputMessage::Clone() const
	{
		return gsl::owner<Scope*>();
	}
}