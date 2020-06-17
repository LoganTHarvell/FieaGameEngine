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

	ActionList& ActionList::operator=(const ActionList& rhs)
	{
		if (this != &rhs)
		{
			Action::operator=(rhs);
		}

		return *this;
	}

	ActionList& ActionList::operator=(ActionList&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Action::operator=(std::move(rhs));
		}

		return *this;
	}

	ActionList::ActionList(const RTTI::IdType typeId, const std::string& name) : Action(typeId, name),
		mActions(mPairPtrs[ActionsIndex]->second)
	{
	}

	gsl::owner<Scope*> ActionList::Clone() const
	{
		return new ActionList(*this);
	}

	Action::Data& ActionList::Actions()
	{
		return mActions;
	}

	const Action::Data& ActionList::Actions() const
	{
		return mActions;
	}

	Action* ActionList::CreateAction(const std::string& className, const std::string& name)
	{
		Scope* newScope = Factory<Scope>::Create(className);

		if (newScope)
		{
			assert(newScope->Is(Action::TypeIdClass()));

			Action* newAction = static_cast<Action*>(newScope);
			newAction->SetName(name);

			Adopt(*newScope, ActionsKey);
			return newAction;
		}

		return nullptr;
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