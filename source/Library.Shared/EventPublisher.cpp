#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "EventPublisher.h"

// Standard
#include <future>

// First Party
#include "IEventSubscriber.h"
#include "Utility.h"
#pragma endregion Includes

using namespace std::string_literals;

namespace Library
{
	void EventPublisher::Publish()
	{
		assert(mSubscriberList);
		if (!mSubscriberList) return;
		
		SubscriberList subscribers;
		{
			std::scoped_lock<std::mutex> lock(*mMutex);
			subscribers = *mSubscriberList;
		}
		
		for (auto* subscriber : subscribers)
		{
			assert(subscriber);
			subscriber->Notify(*this);
		}
	}
}