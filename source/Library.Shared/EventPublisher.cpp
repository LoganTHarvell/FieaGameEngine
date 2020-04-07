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
		assert(mSubscribers);

		for (auto subscriber : *mSubscribers)
		{
			subscriber->Notify(*this);
		}
	}
}