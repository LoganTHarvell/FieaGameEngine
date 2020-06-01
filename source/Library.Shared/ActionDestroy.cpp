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

	void ActionDestroy::Update(WorldState&)
	{
		Entity* parent = GetParent();
		
		if (parent != nullptr)
		{
			Entity* child = parent->FindChild(mTargetName);

			if (child != nullptr)
			{
				parent->DestroyChild(*child);
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