#pragma region Includes
#include "pch.h"
#include "ActionPickupPowerUp.h"

// First Party
#include "World.h"
#include "Sector.h"
#include "Entity.h"

#include "Character.h"
#include "WorldHelpers.h"

#include "AudioManager.h"

#pragma endregion Includes

using namespace Library;

namespace GameDesktop
{
	const TypeManager::TypeInfo& ActionPickupPowerUp::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ "Nothing", Types::String, false, 1, 0 }
			},

			Action::TypeIdClass()
		};

		return typeInfo;
	}

	ActionPickupPowerUp::ActionPickupPowerUp(const std::string& name) 
		: Action(TypeIdClass(), name)
	{
	}

	gsl::owner<Scope*> ActionPickupPowerUp::Clone() const
	{
		return new ActionPickupPowerUp(*this);
	}

	void ActionPickupPowerUp::Update(WorldState& worldState)
	{
		Datum* nameDatum = Search(PowerUp::CharacterNameKey);
		Datum* powerDatum = Search(PowerUp::PowerUpKey);
		if (nameDatum == nullptr || powerDatum == nullptr) // Valid Message Check 
			return;

		Character* myCharacter = WorldHelpers::WorldSearchCharacter(worldState, nameDatum->Get<std::string>());
		PowerUp::EPowerUp power = PowerUp::StringToEPowerUp(Search(PowerUp::PowerUpKey)->Get<std::string>());

		myCharacter->PickupPower(power);

		AudioManager::Instance()->PlaySound("PowerUp");
	}

	std::string ActionPickupPowerUp::ToString() const
	{
		return "ActionPickupPowerUp"; // Unused
	}
}