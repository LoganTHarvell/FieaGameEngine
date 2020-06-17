#include "pch.h"
#include "ActionAnimate.h"
#include "WorldState.h"
#include "GameTime.h"


namespace GameDesktop
{
	const TypeManager::TypeInfo& GameDesktop::ActionAnimate::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{IsEnabledKey, Types::Integer, false, 1, offsetof(ActionAnimate, mIsEnabled)},
				{DelayKey, Types::Integer, false, 1, offsetof(ActionAnimate, mDelay)}
			},
			Action::TypeIdClass()
		};
		return typeInfo;
	}

	gsl::owner<ActionAnimate*> ActionAnimate::Clone() const
	{
		return new ActionAnimate(*this);
	}

	void ActionAnimate::Update(WorldState& worldState)
	{
		if (!mIsEnabled)
		{
			return;
		}

		if (worldState.GameTime->CurrentTime() - mTimeLastUpdated >= std::chrono::milliseconds(mDelay))
		{
			Data* component = Search(GameObject::RenderComponentKey);
			assert(component != nullptr);

			assert(component->Get<Scope*>()->Is(RenderComponent::TypeIdClass()));
			RenderComponent* renderComponent = static_cast<RenderComponent*>(component->Get<Scope*>());

			int32_t currentFrame = renderComponent->GetColumnIndex();
			++currentFrame;
			renderComponent->SetColumnIndex((currentFrame >= renderComponent->GetColumnCount() ? (0) : currentFrame));

			mTimeLastUpdated = std::chrono::high_resolution_clock::now();
		}
	}

	std::string ActionAnimate::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionAnimate)";
		return oss.str();
	}

	void ActionAnimate::Reset(WorldState& worldState)
	{
		mTimeLastUpdated = worldState.GameTime->CurrentTime();
	}

	void ActionAnimate::SetDelay(int newDelay)
	{
		mDelay = newDelay;
	}

	ActionAnimate::ActionAnimate(const std::string& name) : Action(TypeIdClass(), name)
	{
	}

	ActionAnimate::ActionAnimate(int delay, const std::string& name) : 
		Action(TypeIdClass(), name), mDelay(delay)
	{
	}
}
