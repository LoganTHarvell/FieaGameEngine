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
#pragma endregion Includes

namespace Library
{
	void EventQueue::Publish(EventPublisher& event)
	{
		event.Publish();
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		Vector publishThreads(Vector<std::future<void>>::EqualityFunctor{});

		{
			std::scoped_lock<std::mutex> lock(mMutex);
			
			for (EventEntry& entry : mQueue)
			{
				if (gameTime.CurrentTime() >= entry.ExpireTime)
				{
					assert(entry.Publisher);
					publishThreads.EmplaceBack(std::async(std::launch::async, &EventPublisher::Publish, entry.Publisher));
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
	}
}