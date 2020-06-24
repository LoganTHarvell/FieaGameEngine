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
	const SignatureList& ActionDestroy::Signatures()
	{
		static const SignatureList signatures =
		{
			{ TargetKey, Types::String, false, 1, offsetof(ActionDestroy, mTargetName) }
		};

		return signatures;
	}

	ActionDestroy::ActionDestroy(std::string name) : Entity(TypeIdClass(), std::move(name))
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