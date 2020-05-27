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
	const TypeManager::TypeInfo& ActionListWhile::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ ConditionKey, Types::Integer, false, 1, offsetof(ActionListWhile, mCondition) }
			},

			Entity::TypeIdClass()
		};

		return typeInfo;
	}

	ActionListWhile::ActionListWhile(const std::string& name) : Entity(TypeIdClass(), name)
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