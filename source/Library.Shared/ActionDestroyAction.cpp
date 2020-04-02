#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ActionDestroyAction.h"

// First Party
#include "World.h"
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& ActionDestroyAction::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ AttributeNameKey, Types::String, false, 1, offsetof(ActionDestroyAction, mAttributeName) },
				{ TargetKey, Types::String, false, 1, offsetof(ActionDestroyAction, mTargetName) }
			},

			Action::TypeIdClass()
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
			DataType* attribute = worldState.Entity->Find(mAttributeName);

			if (attribute)
			{
				for (std::size_t i = 0; i < attribute->Size(); ++i)
				{
					Scope& scope = (*attribute)[i];
					DataType* name = scope.Find("Name");

					if (name && *name == mTargetName)
					{
						World::PendingChild childToRemove =
						{
							scope,
							World::PendingChild::State::ToRemove,
							*worldState.Entity,
							nullptr
						};

						worldState.World->PendingChildren().PushBack(childToRemove);
					}
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