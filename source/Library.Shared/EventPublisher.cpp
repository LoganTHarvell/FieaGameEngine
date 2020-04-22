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

		Vector notifyThreads(Vector<std::future<void>>::EqualityFunctor{});
		Vector exceptionEntries(Vector<Exception::AggregateException::Entry>::EqualityFunctor{});

		{
			std::scoped_lock<std::mutex> lock(*mMutex);

			for (auto subscriber : *mSubscriberList)
			{
				assert(subscriber);

				auto notify = [subscriber, this, &exceptionEntries]
				{
					try
					{
						subscriber->Notify(*this);
					}
					catch (...)
					{

						Exception::AggregateException::Entry exceptionEntry =
						{
							std::current_exception(),
							(__FILE__),
							(__LINE__),
							"Notify"s,
							ToString()
						};
						
						exceptionEntries.EmplaceBack(std::move(exceptionEntry));
					}
				};
				
				notifyThreads.EmplaceBack(std::async(std::launch::async, notify));
			}
		}

		for (const auto& t : notifyThreads)
		{
			t.wait();
		}

		if (exceptionEntries.Size() > 0)
		{
			throw Exception::AggregateException("Publishing exceptions.", exceptionEntries);
		}
	}
}