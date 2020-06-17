#include "pch.h"
#include "Player1InputMessage.h"
#include "Scope.h"

using namespace Library;

namespace GameDesktop
{
	Player1InputMessage::Player1InputMessage(MessageType type) : PlayerInputMessage(type) {}

	gsl::owner<Scope*> Player1InputMessage::Clone() const
	{
		return new Player1InputMessage(*this);
	}

}