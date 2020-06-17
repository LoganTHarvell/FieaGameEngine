#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ActionDestroy.h"

// First Party
#include "World.h"
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& ActionDestroy::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ AttributeKey, Types::String, false, 1, offsetof(ActionDestroy, mAttributeKey) },
				{ TargetKey, Types::String, false, 1, offsetof(ActionDestroy, mTargetName) }
			},

			Action::TypeIdClass()
		};

		return typeInfo;
	}

	ActionDestroy::ActionDestroy(const std::string& name) : Action(TypeIdClass(), name)
	{
	}

	gsl::owner<Scope*> ActionDestroy::Clone() const
	{
		return new ActionDestroy(*this);
	}

	void ActionDestroy::Update(WorldState& worldState)
	{
		if (worldState.Entity)
		{
			Data* attribute = worldState.Entity->Find(mAttributeKey);

			if (attribute)
			{
				for (std::size_t i = 0; i < attribute->Size(); ++i)
				{
					Scope& scope = (*attribute)[i];
					Data* name = scope.Find("Name");

					if (name && *name == mTargetName)
					{
						World::PendingChild childToRemove =
						{
							scope,
							World::PendingChild::State::ToRemove,
							*worldState.Entity,
							std::string()
						};

						worldState.World->PendingChildren().EmplaceBack(std::move(childToRemove));
					}
				}
			}
		}
	}

	std::string ActionDestroy::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionDestroy)";
		return oss.str();
	}
}