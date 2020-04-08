#include "EventQueue.h"

namespace Library
{
	inline bool EventQueue::EventEntry::operator==(const EventEntry& rhs) const noexcept
	{
		return !this->EventPublisher.expired() && !rhs.EventPublisher.expired()
			&& this->EventPublisher.lock() == rhs.EventPublisher.lock();
	}

	inline bool EventQueue::EventEntry::operator!=(const EventEntry& rhs) const noexcept
	{
		return !this->EventPublisher.expired() && !rhs.EventPublisher.expired()
			&& this->EventPublisher.lock() != rhs.EventPublisher.lock();
	}

	inline std::size_t EventQueue::Size() const
	{
		return mQueue.Size();
	}

	inline bool EventQueue::IsEmpty() const
	{
		return mQueue.IsEmpty();
	}

	inline std::size_t EventQueue::Capacity() const
	{
		return mQueue.Capacity();
	}
	
	inline void EventQueue::Enqueue(const std::weak_ptr<EventPublisher> eventPublisher, const GameTime& gameTime, Duration delay)
	{
		EventEntry entry = { eventPublisher, gameTime.CurrentTime() + delay, false };

		if (mQueue.Find(entry) != mQueue.end())
		{
			throw std::runtime_error("Event already added.");
		}

		mQueue.PushBack(entry);
	}

	inline void EventQueue::Clear()
	{
		mQueue.Clear();
	}
	
	inline void EventQueue::ShrinkToFit()
	{
		mQueue.ShrinkToFit();
	}
}