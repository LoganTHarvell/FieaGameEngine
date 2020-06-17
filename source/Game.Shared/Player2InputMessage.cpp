#include "pch.h"
#include "Player2InputMessage.h"

using namespace Library;

namespace GameDesktop
{
	Player2InputMessage::Player2InputMessage(MessageType type) : PlayerInputMessage(type) {}

	gsl::owner<Scope*> Player2InputMessage::Clone() const
	{
		return gsl::owner<Scope*>();
	}
}