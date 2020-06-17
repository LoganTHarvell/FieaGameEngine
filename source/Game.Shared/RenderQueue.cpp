#include "pch.h"
#include "RenderQueue.h"

namespace GameDesktop
{
	constexpr bool RenderQueue::QueueCompare::operator()(const QueueEntry& lhs, const QueueEntry& rhs) const
	{
		return lhs.first > rhs.first;
	}
	void RenderQueue::Insert(const GameObject* gameObject, int priority)
	{
		mQueue.emplace(QueueEntry(priority, gameObject));
	}
	const GameObject* RenderQueue::Top() const
	{
		return mQueue.top().second;
	}
	void RenderQueue::Pop()
	{
		mQueue.pop();
	}
	bool RenderQueue::IsEmpty() const
	{
		return mQueue.empty();
	}
}