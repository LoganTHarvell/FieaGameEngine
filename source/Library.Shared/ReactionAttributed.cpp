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
	ReactionAttributed::ReactionAttributed(const std::string& name, const Subtype& subtype) : Reaction(TypeIdClass(), name),
		mSubtype(subtype)
	{
	}

	ReactionAttributed::ReactionAttributed(const RTTI::IdType typeId, const std::string& name, const Subtype& subtype) : Reaction(typeId, name),
		mSubtype(subtype)
	{
	}

	gsl::owner<Library::Scope*> ReactionAttributed::Clone() const
	{
		return new ReactionAttributed(*this);
	}

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

	ReactionAttributed::DataType* ReactionAttributed::Search(const KeyType& key, Scope** scopePtrOut)
	{
		DataType* result = nullptr;

		if (!mParameterStack.IsEmpty())
		{
			result = mParameterStack.Top().Find(key);
			if (scopePtrOut) scopePtrOut = nullptr;
		}
		
		return result ? result : ActionList::Search(key, scopePtrOut);
	}
}
