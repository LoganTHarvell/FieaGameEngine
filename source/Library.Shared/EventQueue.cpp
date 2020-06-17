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

using namespace std::string_literals;

namespace Library
{
	void EventQueue::Publish(EventPublisher& event)
	{
		event.Publish();
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		for (EventEntry& entry : mQueue)
		{
			if (gameTime.CurrentTime() >= entry.ExpireTime)
			{
				assert(entry.Publisher);
				entry.IsExpired = true;
			}
		}

		auto isExpired = [&gameTime](const EventEntry& eventEntry) 
		{ 
			return !eventEntry.IsExpired; 
		};

		const auto eventPartition = std::partition(mQueue.begin(), mQueue.end(), isExpired);

		Vector expiredEvents(Vector<EventEntry>::EqualityFunctor{});
		expiredEvents.Insert(expiredEvents.begin(), eventPartition, mQueue.cend());

		mQueue.Erase(eventPartition);
		
		for (const auto& event : expiredEvents)
		{
			event.Publisher->Publish();
		}
	}
}