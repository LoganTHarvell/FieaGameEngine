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
	const SignatureList& ActionCreate::Signatures()
	{
		static const SignatureList signatures =
		{
			{ EntityPrototypeKey, Types::Scope, true, 1, 0 }
		};

		return signatures;
	}

	ActionCreate::ActionCreate(std::string name) : Entity(TypeIdClass(), std::move(name))
	{
	}

	gsl::owner<Scope*> ActionCreate::Clone() const
	{
		return new ActionCreate(*this);
	}

	void ActionCreate::Update(WorldState&)
	{
		Entity* parent = GetParent();
		
		if (parent != nullptr)
		{
			Entity* child = FindChild(EntityPrototypeKey);

			if (child != nullptr)
			{
				parent->AddChild(static_cast<Entity&>(*child->Clone()));
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