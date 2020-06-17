#include "Event.h"

namespace Library
{
#pragma region Static Members
	template<typename MessageT>
	inline std::size_t Event<MessageT>::SubscriberCount()
	{
		return sSubscriberList.Size() + sSubscribersPendingAdd.Size();
	}

	template<typename MessageT>
	inline std::size_t Event<MessageT>::SubscriberCapacity()
	{
		return sSubscriberList.Capacity();
	}

	template<typename MessageT>
	inline void Event<MessageT>::Subscribe(IEventSubscriber& eventSubscriber)
	{
		SubscriberEntry entry = { &eventSubscriber, false };

		if ((sSubscriberList.Find(entry) != sSubscriberList.end() && !sSubscriberList.Find(entry)->PendingRemove) 
		||	sSubscribersPendingAdd.Find(entry) != sSubscribersPendingAdd.end())
		{
			throw std::runtime_error("Subscriber already added.");
		}

		sSubscribersPendingAdd.EmplaceBack(entry);
	}

	template<typename MessageT>
	inline void Event<MessageT>::Unsubscribe(IEventSubscriber& eventSubscriber)
	{
		auto entry = sSubscriberList.Find({ &eventSubscriber, false });
	
		if (entry != sSubscriberList.end())
		{
			entry->PendingRemove = true;
		}
		else
		{
			entry = sSubscribersPendingAdd.Find({ &eventSubscriber, false });
			
			if (entry != sSubscribersPendingAdd.end())
			{
				entry->PendingRemove = true;
			}
		}		
	}

	template<typename MessageT>
	inline void Event<MessageT>::UnsubscribeAll()
	{
		sSubscriberList.Clear();
		sSubscribersPendingAdd.Clear();
	}

	template<typename MessageT>
	inline void Event<MessageT>::SubscriberShrinkToFit()
	{
		sSubscriberList.ShrinkToFit();
		sSubscribersPendingAdd.ShrinkToFit();
	}
#pragma endregion Static Members

#pragma region Special Members
	template<typename MessageT>
	inline Event<MessageT>::Event(MessageT message) : EventPublisher(sSubscriberList, sSubscribersPendingAdd),
		Message(message)
	{
	}
#pragma endregion Special Members

#pragma region RTTI Overrides
	template<typename MessageT>
	std::string Event<MessageT>::ToString() const
	{
		std::ostringstream oss;
		oss << "Event";
		return oss.str();
	}
#pragma endregion RTTI Overrides
}