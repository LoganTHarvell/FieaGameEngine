#include "pch.h"
#include "Player4InputMessage.h"

using namespace Library;

namespace GameDesktop
{
	Player4InputMessage::Player4InputMessage(MessageType type) : PlayerInputMessage(type) {}

	gsl::owner<Scope*> Player4InputMessage::Clone() const
	{
		return gsl::owner<Scope*>();
	}
}