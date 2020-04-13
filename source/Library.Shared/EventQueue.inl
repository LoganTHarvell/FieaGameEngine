#include "EventQueue.h"

namespace Library
{
#pragma region Event Entry
	inline EventQueue::EventEntry::EventEntry(const EventEntry& rhs) :
		Publisher(rhs.Publisher), ExpireTime(rhs.ExpireTime), IsExpired(rhs.IsExpired)
	{
	}

	inline EventQueue::EventEntry& EventQueue::EventEntry::operator=(const EventEntry& rhs)
	{
		Publisher = rhs.Publisher;
		ExpireTime = rhs.ExpireTime;
		IsExpired = rhs.IsExpired;

		return *this;
	}

	inline EventQueue::EventEntry::EventEntry(EventEntry&& rhs) noexcept : 
		Publisher(rhs.Publisher), ExpireTime(rhs.ExpireTime), IsExpired(rhs.IsExpired)
	{
		rhs.Publisher = nullptr;
		rhs.ExpireTime = std::chrono::high_resolution_clock::time_point();
		IsExpired = false;
	}

	inline EventQueue::EventEntry& EventQueue::EventEntry::operator=(EventEntry&& rhs) noexcept
	{
		Publisher = rhs.Publisher;
		ExpireTime = rhs.ExpireTime;
		IsExpired = rhs.IsExpired;

		rhs.Publisher = nullptr;
		rhs.ExpireTime = TimePoint();
		IsExpired = false;

		return *this;
	}

	inline EventQueue::EventEntry::EventEntry(const std::shared_ptr<EventPublisher> publisher, const TimePoint expireTime, const bool isExpired) :
		Publisher(publisher), ExpireTime(expireTime), IsExpired(isExpired) 
	{
	}

	inline bool EventQueue::EventEntry::operator==(const EventEntry& rhs) const noexcept
	{
		return Publisher == rhs.Publisher;
	}

	inline bool EventQueue::EventEntry::operator!=(const EventEntry& rhs) const noexcept
	{
		return !operator==(rhs);
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
	inline void EventQueue::Enqueue(const std::shared_ptr<EventPublisher> eventPublisher, const GameTime& gameTime, Duration delay)
	{
		EventEntry entry(eventPublisher, gameTime.CurrentTime() + delay, false);

		if ((mQueue.Find(entry) != mQueue.end() && !mQueue.Find(entry)->IsExpired) || mPendingQueue.Find(entry) != mPendingQueue.end())
		{
			throw std::runtime_error("Event already added.");
		}

		mUpdating ? mPendingQueue.PushBack(entry) : mQueue.PushBack(entry);
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