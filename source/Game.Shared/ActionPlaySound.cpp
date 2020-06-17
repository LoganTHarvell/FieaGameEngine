#include "pch.h"
#include "ActionPlaySound.h"
#include "WorldState.h"
#include "GameTime.h"
#include "AudioManager.h"

namespace GameDesktop
{
	const TypeManager::TypeInfo& GameDesktop::ActionPlaySound::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{IsEnabledKey, Types::Integer, false, 1, offsetof(ActionPlaySound, mIsEnabled)},
				{DelayKey, Types::Integer, false, 1, offsetof(ActionPlaySound, mDelay)},
				{SoundKey, Types::String, false, 1, offsetof(ActionPlaySound, mSoundName)}
			},
			Action::TypeIdClass()
		};
		return typeInfo;
	}

	gsl::owner<ActionPlaySound*> ActionPlaySound::Clone() const
	{
		return new ActionPlaySound(*this);
	}

	void ActionPlaySound::Update(WorldState& worldState)
	{
		if (!mIsEnabled)
		{
			return;
		}

		if (worldState.GameTime->CurrentTime() - mTimeLastUpdated >= std::chrono::milliseconds(mDelay))
		{
			/*Data* component = Search(GameObject::RenderComponentKey);
			assert(component != nullptr);

			assert(component->Get<Scope*>()->Is(RenderComponent::TypeIdClass()));
			RenderComponent* renderComponent = static_cast<RenderComponent*>(component->Get<Scope*>());*/

			AudioManager::Instance()->PlaySound(mSoundName);

			mTimeLastUpdated = std::chrono::high_resolution_clock::now();
		}
	}

	std::string ActionPlaySound::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionPlaySound)";
		return oss.str();
	}

	ActionPlaySound::ActionPlaySound(const std::string& name) : Action(TypeIdClass(), name)
	{
	}

	ActionPlaySound::ActionPlaySound(int delay, const std::string& name) :
		Action(TypeIdClass(), name), mDelay(delay)
	{
	}
}
