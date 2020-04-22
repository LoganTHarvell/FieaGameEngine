#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "EventPublisher.h"

// Standard
#include <future>

// First Party
#include "IEventSubscriber.h"
#pragma endregion Includes

namespace Library
{
	void EventPublisher::Publish()
	{
		assert(mSubscriberList);

		Vector notifyThreads(Vector<std::future<void>>::EqualityFunctor{});
		
		{
			std::scoped_lock<std::mutex> lock(*mMutex);

			for (auto* subscriber : *mSubscriberList)
			{
				assert(subscriber);
				notifyThreads.EmplaceBack(std::async(std::launch::async, [&subscriber, this] { subscriber->Notify(*this); }));
			}
		}

		for (const auto& t : notifyThreads)
		{
			t.wait();
		}
	}
}