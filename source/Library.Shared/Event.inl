#include "Event.h"

namespace Library
{
	template<typename MessageT>
	inline void Event<MessageT>::Subscribe(IEventSubscriber& eventSubscriber)
	{
		sSubscribers->PushBack(&eventSubscriber);
	}

	template<typename MessageT>
	inline void Event<MessageT>::Unsubscribe(IEventSubscriber& eventSubscriber)
	{
		sSubscribers->Remove(&eventSubscriber);
	}

	template<typename MessageT>
	inline void Event<MessageT>::UnsubscribeAll()
	{
		sSubscribers->Clear();
	}

	template<typename MessageT>
	inline Event<MessageT>::Event(MessageT message) : EventPublisher(sSubscribers),
		mMessage(mMessage)
	{
	}

	template<typename MessageT>
	inline Event<MessageT>::Event(MessageT message)
	{
		mMessage = message;
	}

	template<typename MessageT>
	inline MessageT& Event<MessageT>::Message()
	{
		return mMessage;
	}
	
	template<typename MessageT>
	inline const MessageT& Event<MessageT>::Message() const
	{
		return mMessage;
	}
}