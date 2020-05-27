#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ActionCreate.h"

// First Party
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& ActionCreate::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ AttributeKey, Types::String, false, 1, offsetof(ActionCreate, mAttributeKey) },
				{ NewScopeKey, Types::Scope, true, 1, 0 }
			},

			Entity::TypeIdClass()
		};

		return typeInfo;
	}

	ActionCreate::ActionCreate(const std::string& name) : Entity(TypeIdClass(), name)
	{
	}

	gsl::owner<Scope*> ActionCreate::Clone() const
	{
		return new ActionCreate(*this);
	}

	void ActionCreate::Update(WorldState& worldState)
	{
		if (worldState.World && worldState.Entity)
		{
			Data* scope = Find(NewScopeKey);

			if (scope && scope->Type() == Types::Scope && scope->Size() > 0)
			{
				assert(scope->Get<Scope*>()->Is(Entity::TypeIdClass()));
				worldState.Entity->AddChild(static_cast<Entity&>(*scope->Get<Scope*>()->Clone()));
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