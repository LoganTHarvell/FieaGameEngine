#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ActionCreate.h"

// First Party
#include "World.h"
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& ActionCreate::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ AttributeNameKey, Types::String, false, 1, offsetof(ActionCreate, mAttributeName) },
				{ NewScopeKey, Types::Scope, true, 1, 0 }
			},

			Action::TypeIdClass()
		};

		return typeInfo;
	}

	ActionCreate::ActionCreate(const std::string& name) : Action(TypeIdClass(), name)
	{
	}

	gsl::owner<Library::Scope*> ActionCreate::Clone() const
	{
		return new ActionCreate(*this);
	}

	void ActionCreate::Update(WorldState& worldState)
	{
		if (worldState.World && worldState.Entity)
		{
			DataType* action = Find(NewScopeKey);

			if (action && action->Type() == Types::Scope && action->Size() > 0)
			{
				World::PendingChild childToAdd =
				{
					*action->Get<Scope*>()->Clone(),
					World::PendingChild::State::ToAdd,
					*worldState.Entity,
					&mAttributeName
				};

				worldState.World->PendingChildren().PushBack(childToAdd);
			}
		}
	}

	std::string ActionCreate::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionCreate)";
		return oss.str();
	}
}