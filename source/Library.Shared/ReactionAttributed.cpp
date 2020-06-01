#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ReactionAttributed.h"

#include <utility>

// First Party
#include "EventMessageAttributed.h"
#include "EventPublisher.h"
#include "Event.h"
#include "World.h"
#pragma endregion Includes

namespace Library
{
#pragma region Static Members
	const TypeManager::TypeInfo& ReactionAttributed::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ SubtypeKey, Types::String, false, 1, offsetof(ReactionAttributed, mSubtype) },
			},

			Reaction::TypeIdClass()
		};

		return typeInfo;
	}
#pragma endregion


#pragma region Special Members
	ReactionAttributed::ReactionAttributed(std::string name, Subtype subtype) : Reaction(TypeIdClass(), std::move(name)),
		mSubtype(std::move(subtype))
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const ReactionAttributed& rhs) : Reaction(rhs)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(ReactionAttributed&& rhs) noexcept : Reaction(rhs)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	gsl::owner<Scope*> ReactionAttributed::Clone() const
	{
		return new ReactionAttributed(*this);
	}
#pragma endregion Virtual Copy Constructor

#pragma region Event Subscriber Overrides
	void ReactionAttributed::Notify(EventPublisher& eventPublisher)
	{
		assert(eventPublisher.Is(Event<EventMessageAttributed>::TypeIdClass()));
		auto message = static_cast<Event<EventMessageAttributed>*>(&eventPublisher)->Message;
		
		if (mSubtype == message.GetSubtype() && message.GetWorld())
		{			
			Scope parameters;

			message.ForEachAuxiliary([&](const Attribute& attribute) 
            {
                mParameters[attribute.first] = attribute.second; 
            });
					
			Entity::Update(message.GetWorld()->GetWorldState());
			
			mParameters.Clear();
		}
	}
#pragma endregion Event Subscriber Overrides

#pragma region Scope Overrides
	ReactionAttributed::Data* ReactionAttributed::Find(const Key& key)
	{
		Data* result = nullptr;

		if (!mParameters.IsEmpty())
		{
			result = mParameters.Find(key);
		}
		
		return result ? result : Entity::Find(key);
	}
#pragma endregion Scope Overrides
}
