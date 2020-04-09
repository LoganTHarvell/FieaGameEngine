#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "EventPublisher.h"

// First Party
#include "IEventSubscriber.h"
#pragma endregion Includes

namespace Library
{
	void EventPublisher::Publish()
	{
		assert(mSubscriberList);

		auto pendingRemove = [](const SubscriberEntry& entry)
		{
			return !entry.PendingRemove;
		};

		mSubscriberList->Erase(std::partition(mSubscriberList->begin(), mSubscriberList->end(), pendingRemove));
		
		assert(mSubscribersPendingAdd);
		mSubscriberList->Insert(mSubscriberList->end(), mSubscribersPendingAdd->begin(), mSubscribersPendingAdd->end());
		mSubscribersPendingAdd->Clear();

		for (auto entry : *mSubscriberList)
		{
			assert(entry.Subscriber);
			entry.Subscriber->Notify(*this);
		}
	}
}