#pragma once
#include "Attributed.h"
#include "World.h"

namespace GameDesktop
{
	class PowerUpPickupMessage : public Library::Attributed
	{
		RTTI_DECLARATIONS(PowerUpPickupMessage, Attributed)

	public:
		explicit PowerUpPickupMessage();

		virtual ~PowerUpPickupMessage() = default;
		PowerUpPickupMessage(const PowerUpPickupMessage& rhs) = default;
		PowerUpPickupMessage& operator=(const PowerUpPickupMessage& rhs) = default;
		PowerUpPickupMessage(PowerUpPickupMessage&& rhs) = default;
		PowerUpPickupMessage& operator=(PowerUpPickupMessage&& rhs) = default;

		virtual gsl::owner<Scope*> Clone() const override;

		static const Library::TypeManager::TypeInfo& TypeInfo();
	};
}

