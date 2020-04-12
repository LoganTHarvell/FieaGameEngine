#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ActionEvent.h"

// First Party
#include "World.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& ActionEvent::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ SubtypeKey, Types::String, false, 1, offsetof(ActionEvent, mSubtype) },
				{ DelayKey, Types::Integer, false, 1, offsetof(ActionEvent, mDelay) }
			},

			Action::TypeIdClass()
		};

		return typeInfo;
	}

	ActionEvent::ActionEvent(const std::string& name, const std::string& subtype, const int delay) : Action(TypeIdClass(), name),
		mSubtype(subtype), mDelay(delay)
	{
	}

	gsl::owner<Scope*> ActionEvent::Clone() const
	{
		return new ActionEvent(*this);
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		if (worldState.World)
		{
			mEvent = std::make_shared<Event<EventMessageAttributed>>();
			mEvent->Message().SetWorld(worldState.World);
			mEvent->Message().SetSubtype(mSubtype);

			ForEachAuxiliary([&](const Attribute& attribute)
			{
				mEvent->Message().AppendAuxiliaryAttribute(attribute.first) = attribute.second;
			});

			EventQueue& eventQueue = worldState.World->GetEventQueue();
			eventQueue.Enqueue(mEvent, worldState.GetGameTime(), std::chrono::milliseconds(mDelay));
		}
	}

	std::string ActionEvent::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionEvent)";
		return oss.str();
	}
}