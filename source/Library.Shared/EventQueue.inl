#include "EventQueue.h"

namespace Library
{
#pragma region Event Entry
	inline EventQueue::EventEntry::EventEntry(const std::shared_ptr<EventPublisher> publisher, const TimePoint& expireTime) :
		Publisher(publisher), ExpireTime(expireTime)
	{
	}
#pragma endregion Event Entry

#pragma region Accessors
	inline std::size_t EventQueue::Size() const
	{
		return mQueue.Size() + mPendingQueue.Size();
	}

	inline bool EventQueue::IsEmpty() const
	{
		return mQueue.IsEmpty() && mPendingQueue.IsEmpty();
	}

	inline std::size_t EventQueue::Capacity() const
	{
		return mQueue.Capacity();
	}
#pragma endregion Accessors
	
#pragma region Modifiers
	inline void EventQueue::Enqueue(const std::shared_ptr<EventPublisher> eventPublisher, const TimePoint& expireTime)
	{
		if (!eventPublisher) throw std::runtime_error("Attempted to Enqueue null pointer.");

		EventEntry entry(eventPublisher, expireTime);
		mUpdating ? mPendingQueue.EmplaceBack(entry) : mQueue.EmplaceBack(entry);
	}

	inline void EventQueue::Clear()
	{
		if (mUpdating)
		{
			mPendingClear = true;
		}
		else
		{
			mQueue.Clear();
			mPendingQueue.Clear();
			mPendingClear = false;
		}
	}
	
	inline void EventQueue::ShrinkToFit()
	{
		if (mUpdating)
		{
			mPendingShrinkToFit = true;
		}
		else
		{
			mQueue.ShrinkToFit();
			mPendingQueue.ShrinkToFit();
			mPendingShrinkToFit = false;
		}
	}
#pragma endregion Modifiers
}