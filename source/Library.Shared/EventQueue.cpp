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
	void EventQueue::Update(const GameTime& gameTime)
	{
		for (EventEntry& entry : mQueue)
		{
			auto eventPublisher = entry.EventPublisher.lock();

			if (eventPublisher)
			{
				if (gameTime.CurrentTime() > entry.ExpireTime)
				{
					eventPublisher->Publish();
					entry.IsExpired = true;
				}
			}
			else
			{
				entry.IsExpired = true;
			}
		}

		auto isExpired = [](const EventEntry& eventEntry) 
		{ 
			return !eventEntry.IsExpired; 
		};
		
		// TODO: Figure out how to get std::partition to work
		//mQueue.Erase(std::partition(mQueue.begin(), mQueue.end(), isExpired));
	}
}