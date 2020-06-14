#pragma once

// Header
#include "EventQueue.h"

namespace Library
{
#pragma region Event Entry
	inline EventQueue::EventEntry::EventEntry(std::shared_ptr<EventPublisher> publisher, const TimePoint& expireTime) :
		Publisher(std::move(publisher)), ExpireTime(expireTime)
	{
	}
#pragma endregion Event Entry

#pragma region Accessors
	inline std::size_t EventQueue::Size() const
	{
		std::scoped_lock<std::mutex> lock(mMutex);
		return mQueue.Size();
	}

	inline bool EventQueue::IsEmpty() const
	{
		std::scoped_lock<std::mutex> lock(mMutex);
		return mQueue.IsEmpty();
	}

	inline std::size_t EventQueue::Capacity() const
	{
		std::scoped_lock<std::mutex> lock(mMutex);
		return mQueue.Capacity();
	}
#pragma endregion Accessors
	
#pragma region Modifiers
	inline void EventQueue::Enqueue(const std::shared_ptr<EventPublisher>& eventPublisher, const TimePoint& expireTime)
	{
		std::scoped_lock<std::mutex> lock(mMutex);

		if (!eventPublisher) throw std::runtime_error("Attempted to Enqueue null pointer.");

		mQueue.EmplaceBack(EventEntry(eventPublisher, expireTime));
	}

	inline void EventQueue::Clear()
	{
		std::scoped_lock<std::mutex> lock(mMutex);

		mQueue.Clear();
	}
	
	inline void EventQueue::ShrinkToFit()
	{
		std::scoped_lock<std::mutex> lock(mMutex);
						
		mQueue.ShrinkToFit();
	}
#pragma endregion Modifiers
}