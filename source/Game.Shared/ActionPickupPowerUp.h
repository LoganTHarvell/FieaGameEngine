#pragma once

#pragma region Includes
// First Party
#include "Action.h"
#include "WorldState.h"
#include "Factory.h"
#pragma endregion Includes

using namespace Library;

namespace GameDesktop
{
	/// <summary>
	/// Action occures when the player collides with a powerup
	/// 
	/// CharacterName // Searches through the current sector for the matching name
	/// PowerUp(string) // Is the string of the EPowerUp
	/// 
	/// </summary>

	class ActionPickupPowerUp final : public Action
	{
		RTTI_DECLARATIONS(ActionPickupPowerUp, Action)

	public:
		static const TypeManager::TypeInfo& TypeInfo();

		explicit ActionPickupPowerUp(const std::string& name = std::string());

		~ActionPickupPowerUp() = default;
		ActionPickupPowerUp(const ActionPickupPowerUp& rhs) = default;
		ActionPickupPowerUp& operator=(const ActionPickupPowerUp& rhs) = default;
		ActionPickupPowerUp(ActionPickupPowerUp&& rhs) noexcept = default;
		ActionPickupPowerUp& operator=(ActionPickupPowerUp&& rhs) noexcept = default;

		virtual gsl::owner<Scope*> Clone() const override;

		virtual void Update(WorldState&);

		virtual std::string ToString() const override;
	};

	ConcreteFactory(ActionPickupPowerUp, Scope)
}

