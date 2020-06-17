#pragma once

// Header
#include "EventMessageAttributed.h"

namespace Library
{
#pragma region Accessors
	inline World* EventMessageAttributed::GetWorld() const
	{
		return mWorld;
	}

	inline void EventMessageAttributed::SetWorld(World* world)
	{
		mWorld = world;
	}

	inline std::string& EventMessageAttributed::GetSubtype()
	{
		return mSubtype;
	}

	inline const std::string& EventMessageAttributed::GetSubtype() const
	{
		return mSubtype;
	}

	inline void EventMessageAttributed::SetSubtype(const Key& subtype)
	{
		mSubtype = subtype;
	}
#pragma endregion Accessors
}