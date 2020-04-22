#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "EventQueue.h"

// Standard
#include <algorithm>
#include <future>

// First Party
#include "EventPublisher.h"
#include "Utility.h"
#pragma endregion Includes

using namespace std::string_literals;

namespace Library
{
	void EventQueue::Publish(EventPublisher& event)
	{
		event.Publish();
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		Vector publishThreads(Vector<std::future<void>>::EqualityFunctor{});
		Vector exceptionEntries(Vector<Exception::AggregateException::Entry>::EqualityFunctor{});

		{
			std::scoped_lock<std::mutex> lock(mMutex);
			
			for (EventEntry& entry : mQueue)
			{
				if (gameTime.CurrentTime() >= entry.ExpireTime)
				{
					assert(entry.Publisher);

					auto publish = [&entry, this, &exceptionEntries]
					{
						try
						{
							entry.Publisher->Publish();
						}
						catch (...)
						{
							Exception::AggregateException::Entry exceptionEntry =
							{
								std::current_exception(),
								(__FILE__),
								(__LINE__),
								"Publish"s,
								entry.Publisher->ToString()
							};

							exceptionEntries.EmplaceBack(std::move(exceptionEntry));
						}
					};
					
					publishThreads.EmplaceBack(std::async(std::launch::async, publish));
					entry.IsExpired = true;
				}
			}
		}

		for (const auto& t : publishThreads)
		{
			t.wait();
		}

		auto isExpired = [&gameTime](const EventEntry& eventEntry) 
		{ 
			return !eventEntry.IsExpired; 
		};
		
		mQueue.Erase(std::partition(mQueue.begin(), mQueue.end(), isExpired));
		
		if (exceptionEntries.Size() > 0)
		{
			throw Exception::AggregateException("Update exceptions.", exceptionEntries);
		}
	}
}