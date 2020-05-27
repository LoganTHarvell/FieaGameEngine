#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ActionEvent.h"

// First Party
#include "World.h"
#include "EventQueue.h"
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

			Entity::TypeIdClass()
		};

		return typeInfo;
	}

	ActionEvent::ActionEvent(const std::string& name, const std::string& subtype, const int delay) : Entity(TypeIdClass(), name),
		mSubtype(subtype), mDelay(delay)
	{
	}

	gsl::owner<Scope*> ActionEvent::Clone() const
	{
		return new ActionEvent(*this);
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		const GameTime* gameTime = worldState.GameTime;

		if (worldState.World && worldState.EventQueue && gameTime)
		{
			EventMessageAttributed message;
			message.SetWorld(worldState.World);
			message.SetSubtype(mSubtype);

			ForEachAuxiliary([&](const Attribute& attribute)
			{
				message.AppendAuxiliaryAttribute(attribute.first) = attribute.second;
			});

			worldState.EventQueue->Enqueue(std::make_shared<Event<EventMessageAttributed>>(message),
										   gameTime->CurrentTime() + std::chrono::milliseconds(mDelay));
		}
	}

	std::string ActionEvent::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionEvent)";
		return oss.str();
	}
}