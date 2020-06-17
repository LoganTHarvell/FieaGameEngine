#pragma once
#include <vector>
#include <queue>
#include <functional>

namespace GameDesktop
{
	class GameObject;
	class RenderQueue final
	{
	public:
		using QueueEntry = std::pair<int, const GameObject*>;
	private:
		struct QueueCompare
		{
			constexpr bool operator()(const QueueEntry& lhs, const QueueEntry& rhs) const;
		};

	public:
		void Insert(const GameObject* gameObject, int priority);
		const GameObject* Top() const;
		void Pop();
		bool IsEmpty() const;
	private:
		std::priority_queue<QueueEntry, std::vector<QueueEntry>, QueueCompare> mQueue;
	};
}

