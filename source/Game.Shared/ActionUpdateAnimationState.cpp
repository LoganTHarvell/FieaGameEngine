#include "pch.h"
#include "ActionUpdateAnimationState.h"

#include "PlayerInputReaction.h"

#include "Character.h"

namespace GameDesktop
{
	const TypeManager::TypeInfo& GameDesktop::ActionUpdateAnimationState::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{IsEnabledKey, Types::Integer, false, 1, offsetof(ActionUpdateAnimationState, mIsEnabled)},
			},
			Action::TypeIdClass()
		};
		return typeInfo;
	}

	gsl::owner<ActionUpdateAnimationState*> GameDesktop::ActionUpdateAnimationState::Clone() const
	{
		return new ActionUpdateAnimationState(*this);
	}

	void ActionUpdateAnimationState::Update(WorldState& worldState)
	{
		if (!mIsEnabled)
		{
			return;
		}

		assert(worldState.Entity->Is(Character::TypeIdClass()));
		Character* character = static_cast<Character*>(worldState.Entity);
		
		if (character->IsDying())
		{
			return;
		}

		float xAxis = Search(PlayerInputReaction::XAxisKey)->Get<float>();
		float yAxis = Search(PlayerInputReaction::YAxisKey)->Get<float>();

		int newRowIndex = 0;

		if (xAxis == 0 && yAxis == 0)
		{
			newRowIndex = mLastDirection;
		}
		else if(xAxis == 1)
		{
			//Right
			newRowIndex = 3;
			mLastDirection = 7;
		}
		else if (xAxis == -1)
		{
			//Left
			newRowIndex = 1;
			mLastDirection = 5;
		}
		else if (yAxis == 1)
		{
			//Down
			newRowIndex = 2;
			mLastDirection = 6;
		}
		else if (yAxis == -1)
		{
			//Up
			newRowIndex = 0;
			mLastDirection = 4;
		}

		Data* component = Search(GameObject::RenderComponentKey);
		assert(component != nullptr);

		assert(component->Get<Scope*>()->Is(RenderComponent::TypeIdClass()));
		RenderComponent* renderComponent = static_cast<RenderComponent*>(component->Get<Scope*>());

		renderComponent->SetRowIndex(newRowIndex);
	}

	std::string GameDesktop::ActionUpdateAnimationState::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionUpdateAnimationState)";
		return oss.str();
	}

	ActionUpdateAnimationState::ActionUpdateAnimationState(const std::string& name) : Action(TypeIdClass(), name)
	{
	}
}

