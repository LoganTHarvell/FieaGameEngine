#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "EventMessageAttributed.h"

// First Party
#include "World.h"
#pragma endregion Includes

namespace Library
{
#pragma region Static Members
	const TypeManager::TypeInfo& EventMessageAttributed::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ SubtypeKey, Types::String, false, 1, offsetof(EventMessageAttributed, mSubtype) }
			},

			Attributed::TypeIdClass()
		};

		return typeInfo;
	}
#pragma endregion Static Members

#pragma region Special Members
	EventMessageAttributed::EventMessageAttributed(World* world, Subtype subtype) : Attributed(TypeIdClass()),
		mWorld(world), mSubtype(std::move(subtype))
	{
	}
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	gsl::owner<Scope*> EventMessageAttributed::Clone() const
	{
		return new EventMessageAttributed(*this);
	}
#pragma endregion Virtual Copy Constructor
}