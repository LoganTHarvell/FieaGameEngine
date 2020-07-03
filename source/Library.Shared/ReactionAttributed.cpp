#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ReactionAttributed.h"

// Standard
#include <iostream>

// First Party
#include "EventMessageAttributed.h"
#include "EventPublisher.h"
#include "Event.h"
#include "World.h"
#pragma endregion Includes

using namespace std::string_literals;

namespace Library
{
#pragma region Static Members
	const SignatureList& ReactionAttributed::Signatures()
	{
		static const SignatureList signatures
		{
			{ SubtypeKey, Types::String, false, 1, offsetof(ReactionAttributed, mSubtype) },
		};

		return signatures;
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
		try
		{
			Event<EventMessageAttributed>::Unsubscribe(*this);
		}
		catch (...)
		{
			std::cerr	<< "ReactionAttributed instance unsubscribe from Event<EventMessageAttributed> on destruction."s
						<< "It could not be found in the Event's subscriber list."s << std::endl;
		}
	}

	ReactionAttributed::ReactionAttributed(const ReactionAttributed& rhs) : Reaction(rhs)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(ReactionAttributed&& rhs) noexcept : Reaction(std::move(rhs))
	{
		try
		{
			Event<EventMessageAttributed>::Subscribe(*this);
		}
		catch (...)
		{
			std::cerr	<< "ReactionAttributed instance subscription to Event<EventMessageAttributed> failed."s
						<< "It is already found on the Event's subscriber list. (Impossible)"s << std::endl;
		}
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
