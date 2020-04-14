#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ReactionAttributed.h"

// First Party
#include "EventMessageAttributed.h"
#include "EventPublisher.h"
#include "Event.h"
#include "World.h"
#pragma endregion Includes

namespace Library
{
#pragma region Special Members
	ReactionAttributed::ReactionAttributed(const std::string& name, const Subtype& subtype) : Reaction(TypeIdClass(), name),
		mSubtype(subtype)
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
		auto message = static_cast<Event<EventMessageAttributed>*>(&eventPublisher)->Message();
		
		if (mSubtype == message.GetSubtype() && message.GetWorld())
		{
			Scope parameters;

			message.ForEachAuxiliary([&](const Attribute& attribute) 
			{
				parameters[attribute.first] = attribute.second; 
			});

			mParameterStack.Push(parameters);
			ActionList::Update(message.GetWorld()->GetWorldState());
			mParameterStack.Pop();
		}
	}
#pragma endregion Event Subscriber Overrides

#pragma region Scope Overrides
	ReactionAttributed::Data* ReactionAttributed::Find(const Key& key)
	{
		Data* result = nullptr;

		if (!mParameterStack.IsEmpty())
		{
			result = mParameterStack.Top().Find(key);
		}
		
		return result ? result : ActionList::Find(key);
	}
#pragma endregion Scope Overrides
}
