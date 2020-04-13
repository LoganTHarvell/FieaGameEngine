#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "EventQueue.h"

// Standard
#include <algorithm>

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
		mUpdating = true;

		for (EventEntry& entry : mQueue)
		{
			assert(entry.Publisher);

			if (gameTime.CurrentTime() >= entry.ExpireTime)
			{
				entry.Publisher->Publish();
				entry.IsExpired = true;
			}
		}

		mUpdating = false;

		auto isExpired = [](const EventEntry& eventEntry) 
		{ 
			return !eventEntry.IsExpired; 
		};
		
		if (mPendingClear)
		{
			Clear();
		}
		else
		{
			mQueue.Erase(std::partition(mQueue.begin(), mQueue.end(), isExpired));
			mQueue.Insert(mQueue.end(), mPendingQueue.begin(), mPendingQueue.end());
			mPendingQueue.Clear();
		}

		if (mPendingShrinkToFit) ShrinkToFit();
	}
}