#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ActionListWhile.h"

// First Party
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	const SignatureList& ActionListWhile::Signatures()
	{
		static const SignatureList signatures =
		{
			{ ConditionKey, Types::Integer, false, 1, offsetof(ActionListWhile, mCondition) }
		};

		return signatures;
	}

	ActionListWhile::ActionListWhile(std::string name) : Entity(TypeIdClass(), std::move(name))
	{
	}

	gsl::owner<Scope*> ActionListWhile::Clone() const
	{
		return new ActionListWhile(*this);
	}

	void ActionListWhile::Update(WorldState& worldState)
	{
		while (mCondition)
		{
			Entity::Update(worldState);
		}
	}

	std::string ActionListWhile::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionListWhile)";
		return oss.str();
	}
}