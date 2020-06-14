#pragma once

// Header
#include "Event.h"

namespace Library
{
#pragma region Static Members
	template<typename MessageT>
	inline std::size_t Event<MessageT>::SubscriberCount()
	{
		std::scoped_lock<std::mutex> lock(sMutex);
		return sSubscriberList.Size();
	}

	template<typename MessageT>
	inline std::size_t Event<MessageT>::SubscriberCapacity()
	{
		std::scoped_lock<std::mutex> lock(sMutex);
		return sSubscriberList.Capacity();
	}

	template<typename MessageT>
	inline void Event<MessageT>::Subscribe(IEventSubscriber& eventSubscriber)
	{
		std::scoped_lock<std::mutex> lock(sMutex);

		IEventSubscriber* subscriber = &eventSubscriber;

		const auto entryIt = sSubscriberList.Find(subscriber);
		
		if (entryIt != sSubscriberList.end())
		{
			throw std::runtime_error("Subscriber already added.");
		}

		sSubscriberList.EmplaceBack(subscriber);
	}

	template<typename MessageT>
	inline void Event<MessageT>::Unsubscribe(IEventSubscriber& eventSubscriber)
	{
		std::scoped_lock<std::mutex> lock(sMutex);

		const auto subscriber = sSubscriberList.Find(&eventSubscriber);
	
		if (subscriber != sSubscriberList.end())
		{
			sSubscriberList.Remove(subscriber);
		}
	}

	template<typename MessageT>
	inline void Event<MessageT>::UnsubscribeAll()
	{
		std::scoped_lock<std::mutex> lock(sMutex);
		sSubscriberList.Clear();
	}

	template<typename MessageT>
	inline void Event<MessageT>::SubscriberShrinkToFit()
	{
		std::scoped_lock<std::mutex> lock(sMutex);
		sSubscriberList.ShrinkToFit();
	}
#pragma endregion Static Members

#pragma region Special Members
	template<typename MessageT>
	inline Event<MessageT>::Event() : EventPublisher(sSubscriberList, sMutex)
	{
	}

	template<typename MessageT>
	inline Event<MessageT>::Event(const MessageT& message) : EventPublisher(sSubscriberList, sMutex),
		Message(message)
	{
	}
	
	template<typename MessageT>
	inline Event<MessageT>::Event(MessageT&& message) : EventPublisher(sSubscriberList, sMutex),
		Message(message)
	{
	}
#pragma endregion Special Members

#pragma region RTTI Overrides
	template<typename MessageT>
	std::string Event<MessageT>::ToString() const
	{
		return "Event";
	}
#pragma endregion RTTI Overrides
}