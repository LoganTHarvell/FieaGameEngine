#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ActionCreateAction.h"

// First Party
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& ActionCreateAction::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ TargetKey, Types::String, false, 1, offsetof(ActionCreateAction, mTarget) },
				{ ActionKey, Types::Scope, true, 1, 0 }
			},

			Attributed::TypeIdClass()
		};

		return typeInfo;
	}

	ActionCreateAction::ActionCreateAction(const std::string& name) : Action(TypeIdClass(), name)
	{
	}

	gsl::owner<Library::Scope*> ActionCreateAction::Clone() const
	{
		return new ActionCreateAction(*this);
	}

	void ActionCreateAction::Update(WorldState& worldState)
	{
		if (worldState.Entity)
		{
			Scope::DataType* action = Find(ActionKey);

			if (action && action->Type() == Types::Scope && action->Size() > 0)
			{
				assert(action->Get<Scope*>()->Is(Action::TypeIdClass()));

				worldState.Entity->Actions().PushBack(action->Get<Scope*>());
			}
		}
	}

	std::string ActionCreateAction::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionCreateAction)";
		return oss.str();
	}
}