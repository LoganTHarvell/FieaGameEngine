#include "EventPublisher.h"

namespace Library
{
	inline EventPublisher::EventPublisher(EventPublisher&& rhs) noexcept :
		mSubscriberList(rhs.mSubscriberList), mMutex(rhs.mMutex)
	{
		rhs.mSubscriberList = nullptr;
		rhs.mMutex = nullptr;
	}

	inline EventPublisher& EventPublisher::operator=(EventPublisher&& rhs) noexcept
	{
		mSubscriberList = rhs.mSubscriberList;
		mMutex = rhs.mMutex;
		
		rhs.mSubscriberList = nullptr;
		rhs.mMutex = nullptr;
		
		return *this;
	}

	inline EventPublisher::EventPublisher(SubscriberList& subscribers, std::mutex& mutex) :
		mSubscriberList(&subscribers), mMutex(&mutex)
	{
	}

	inline std::string EventPublisher::ToString() const
	{
		return "EventPublisher";
	}
}