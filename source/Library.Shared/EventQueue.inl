#include "EventQueue.h"

namespace Library
{
	inline std::size_t EventQueue::Size() const
	{
		return mQueue.Size();
	}

	inline bool EventQueue::IsEmpty() const
	{
		return mQueue.IsEmpty();
	}
	
	inline void EventQueue::Enqueue(const std::weak_ptr<EventPublisher> eventPublisher, const GameTime& gameTime, Duration delay)
	{
		mQueue.PushBack({ eventPublisher, gameTime.CurrentTime() + delay, false });
	}

	inline void EventQueue::Clear()
	{
		mQueue.Clear();
	}
}