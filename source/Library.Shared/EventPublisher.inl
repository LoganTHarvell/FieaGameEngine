#include "EventPublisher.h"

namespace Library
{
	inline bool EventPublisher::SubscriberEntry::operator==(const SubscriberEntry& rhs) const noexcept
	{
		assert(this->Subscriber);
		return this->Subscriber == rhs.Subscriber;
	}

	inline bool EventPublisher::SubscriberEntry::operator!=(const SubscriberEntry& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	inline EventPublisher::EventPublisher(EventPublisher&& rhs) noexcept :
		mSubscriberList(rhs.mSubscriberList), mSubscribersPendingAdd(rhs.mSubscribersPendingAdd)
	{
		rhs.mSubscriberList = nullptr;
		rhs.mSubscribersPendingAdd = nullptr;
	}

	inline EventPublisher& EventPublisher::operator=(EventPublisher&& rhs) noexcept
	{
		mSubscriberList = rhs.mSubscriberList;
		mSubscribersPendingAdd = rhs.mSubscribersPendingAdd;

		rhs.mSubscriberList = nullptr;
		rhs.mSubscribersPendingAdd = nullptr;

		return *this;
	}

	inline EventPublisher::EventPublisher(SubscriberList& subscribers, SubscriberList& subscribersPendingAdd) :
		mSubscriberList(&subscribers), mSubscribersPendingAdd(&subscribersPendingAdd)
	{
	}

	inline std::string EventPublisher::ToString() const
	{
		return "EventPublisher";
	}
}