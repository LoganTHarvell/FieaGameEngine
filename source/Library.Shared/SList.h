#pragma once


#include <memory>


namespace Library
{
	/// <summary>
	/// Represents a generic singly linked list.
	/// </summary>
	template <typename T>
	class SList
	{
	private:
		struct Node final
		{
			T Data;
			std::shared_ptr<Node> Next;
		
			Node(const T& data, std::shared_ptr<Node> next = nullptr);
		};

	public:
		SList() = default;
		SList(const SList& list);
		~SList();

		SList& operator=(const SList& list);
		bool operator==(const SList& list) const;

		bool IsEmpty() const;
		size_t Size() const;

		T& Front();
		const T& Front() const;

		void PushFront(const T& data);
		void PopFront();

		T& Back();
		const T& Back() const;

		void PushBack(const T& data);
		void PopBack();

		void Clear();

	private:
		size_t mSize = 0;
		std::shared_ptr<Node> mFront = nullptr;
		std::shared_ptr<Node> mBack = nullptr;
	};
}

#include "SList.inl"