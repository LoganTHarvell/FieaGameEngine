#include "pch.h"
#include "PowerupPickupMessage.h"

namespace GameDesktop
{
	using namespace Library;

	PowerUpPickupMessage::PowerUpPickupMessage() :
		Attributed(TypeIdClass())
	{
	}

	gsl::owner<Scope*> PowerUpPickupMessage::Clone() const
	{
		return new PowerUpPickupMessage(*this);
	}

	const Library::TypeManager::TypeInfo& PowerUpPickupMessage::TypeInfo()
	{
		static const Library::TypeManager::TypeInfo typeInfo
		{
			{{}},
			Attributed::TypeIdClass()
		};

		return typeInfo;
	}
}