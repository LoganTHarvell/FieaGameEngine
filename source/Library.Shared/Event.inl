#include "Event.h"

namespace Library
{
#pragma region Static Members
	template<typename MessageT>
	inline std::size_t Event<MessageT>::SubscriberCount()
	{
		return sSubscribers.Size();
	}

	template<typename MessageT>
	inline std::size_t Event<MessageT>::SubscriberCapacity()
	{
		return sSubscribers.Capacity();
	}

	template<typename MessageT>
	inline void Event<MessageT>::Subscribe(IEventSubscriber& eventSubscriber)
	{
		if (sSubscribers.Find(&eventSubscriber) != sSubscribers.end())
		{
			throw std::runtime_error("Subscriber already added.");
		}

		sSubscribers.PushBack(&eventSubscriber);
	}

	template<typename MessageT>
	inline void Event<MessageT>::Unsubscribe(IEventSubscriber& eventSubscriber)
	{
		sSubscribers.Remove(&eventSubscriber);
	}

	template<typename MessageT>
	inline void Event<MessageT>::UnsubscribeAll()
	{
		sSubscribers.Clear();
	}

	template<typename MessageT>
	inline void Event<MessageT>::SubscriberShrinkToFit()
	{
		sSubscribers.ShrinkToFit();
	}
#pragma endregion Static Members

#pragma region Special Members
	template<typename MessageT>
	inline Event<MessageT>::Event(MessageT message) : EventPublisher(sSubscribers),
		mMessage(message)
	{
	}
#pragma endregion Special Members

#pragma region Accessors
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
#pragma endregion Accessors

#pragma region RTTI Overrides
	template<typename MessageT>
	std::string Event<MessageT>::ToString() const
	{
		std::ostringstream oss;
		oss << "Event";
		return oss.str();
	}

	template<typename MessageT>
	inline bool Event<MessageT>::Equals(const RTTI* rhs) const
	{
		if (this == rhs)	return true;
		if (!rhs)			return false;

		const Event<MessageT>* rhsEvent = rhs->As<const Event<MessageT>>();
		return rhsEvent && mMessage == rhsEvent->mMessage;
	}
#pragma endregion RTTI Overrides
}