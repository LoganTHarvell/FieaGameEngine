#include "PlayerInputMessage.h"

namespace GameDesktop
{
#pragma region Accessors
	inline PlayerInputMessage::MessageType PlayerInputMessage::GetType()
	{
		return mType;
	}

	inline const PlayerInputMessage::MessageType PlayerInputMessage::GetType() const
	{
		return mType;
	}

	inline void PlayerInputMessage::SetType(MessageType type)
	{
		mType = type;
	}

	inline Library::World* PlayerInputMessage::GetWorld() const
	{
		return mWorld;
	}

	inline void PlayerInputMessage::SetWorld(Library::World* world)
	{
		mWorld = world;
	}
#pragma endregion Accessors
}