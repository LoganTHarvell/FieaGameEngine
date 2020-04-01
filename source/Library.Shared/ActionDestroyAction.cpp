#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ActionDestroyAction.h"

// First Party
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& ActionDestroyAction::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ ActionNameKey, Types::String, false, 1, offsetof(ActionDestroyAction, mActionName) },
			},

			Attributed::TypeIdClass()
		};

		return typeInfo;
	}

	ActionDestroyAction::ActionDestroyAction(const std::string& name) : Action(TypeIdClass(), name)
	{
	}

	gsl::owner<Library::Scope*> ActionDestroyAction::Clone() const
	{
		return new ActionDestroyAction(*this);
	}

	void ActionDestroyAction::Update(WorldState& worldState)
	{
		if (worldState.Entity)
		{
			Scope::DataType& actions = worldState.Entity->Actions();
			
			for (std::size_t i = 0; i < actions.Size(); ++i)
			{
				assert(actions[i].Is(Action::TypeIdClass()));

				Action* action = static_cast<Action*>(&actions[i]);

				if (action->Name() == mActionName)
				{
					delete action->GetParent()->Orphan(*action);
				}
			}
		}
	}

	std::string ActionDestroyAction::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionDestroyAction)";
		return oss.str();
	}
}