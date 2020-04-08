#include "EventPublisher.h"

namespace Library
{
	inline EventPublisher::EventPublisher(EventPublisher&& rhs) noexcept :
		mSubscribers(rhs.mSubscribers)
	{
		rhs.mSubscribers = nullptr;
	}

	inline EventPublisher& EventPublisher::operator=(EventPublisher&& rhs) noexcept
	{
		mSubscribers = rhs.mSubscribers;
		rhs.mSubscribers = nullptr;

		return *this;
	}

	inline EventPublisher::EventPublisher(SubscriberList& subscribers)
	{
		mSubscribers = &subscribers;
	}

	inline std::string EventPublisher::ToString() const
	{
		return "EventPublisher";
	}
}