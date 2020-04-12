#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ActionList.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& ActionList::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ ActionsKey, Types::Scope, true, 1, 0 }
			},

			Action::TypeIdClass()
		};

		return typeInfo;
	}

	ActionList::ActionList(const std::string& name) : Action(TypeIdClass(), name), 
		mActions(mPairPtrs[ActionsIndex]->second)
	{
	}

	ActionList::ActionList(const RTTI::IdType typeId, const std::string& name) : Action(typeId, name),
		mActions(mPairPtrs[ActionsIndex]->second)
	{
	}

	gsl::owner<Scope*> ActionList::Clone() const
	{
		return new ActionList(*this);
	}

	Action::DataType& ActionList::Actions()
	{
		return mActions;
	}

	const Action::DataType& ActionList::Actions() const
	{
		return mActions;
	}

	void ActionList::Update(WorldState& worldState)
	{
		for (std::size_t i = 0; i < mActions.Size(); ++i)
		{
			assert(mActions[i].Is(Action::TypeIdClass()));

			worldState.Action = static_cast<Action*>(mActions.Get<Scope*>(i));
			worldState.Action->Update(worldState);
		}

		worldState.Action = nullptr;
	}

	std::string ActionList::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionList)";
		return oss.str();
	}
}