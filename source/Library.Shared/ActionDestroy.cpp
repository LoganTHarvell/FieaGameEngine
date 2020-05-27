#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ActionDestroy.h"

// First Party
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

			Entity::TypeIdClass()
		};

		return typeInfo;
	}

	ActionDestroy::ActionDestroy(const std::string& name) : Entity(TypeIdClass(), name)
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
						assert(scope.Is(Entity::TypeIdClass()));
						worldState.Entity->DestroyChild(static_cast<Entity&>(scope));
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