#pragma once

#include <memory>
#include <stdexcept>

namespace Library
{
	/// <summary>
	/// Represents a generic singly linked list.
	/// </summary>
	template <typename T>
	class SList
	{
	private:
#pragma region Node
		/// <summary>
		/// Represents an element in a list.
		/// </summary>
		struct Node final
		{
			T Data;
			std::shared_ptr<Node> Next;
		
			Node(const T& data, std::shared_ptr<Node> next = nullptr);
		};
#pragma endregion Node

	public:
#pragma region Iterator
		// TODO: Add Documentation
		class Iterator final
		{
			friend SList;

		private:
			Iterator(const SList<T>* list, std::shared_ptr<Node> node=nullptr);

		public:
			Iterator() = default;
			Iterator(const Iterator& rhs) = default;
			Iterator& operator=(const Iterator& rhs) = default;
			Iterator(Iterator&& rhs) = default;
			Iterator& operator=(Iterator&& rhs) = default;
			~Iterator() = default;

		public:
			T& operator*();
			bool operator==(const Iterator& rhs) const noexcept;
			bool operator!=(const Iterator& rhs) const noexcept;
			Iterator& operator++();
			Iterator operator++(int);

		private:
			const SList* mOwner{ nullptr };
			std::shared_ptr<Node> mNode{ nullptr };
		};
#pragma endregion Iterator

#pragma region ConstIterator
		class ConstIterator final
		{
			friend SList;
			friend Iterator;

		public:
			ConstIterator(const Iterator& iterator);

		private:
			ConstIterator(const SList* list, std::shared_ptr<Node> node = nullptr);

		public:
			ConstIterator() = default;
			ConstIterator(const ConstIterator&) = default;
			ConstIterator& operator=(const ConstIterator&) = default;
			ConstIterator(ConstIterator&&) = default;
			ConstIterator& operator=(ConstIterator&&) = default;
			~ConstIterator() = default;

		public:
			const T& operator*();
			bool operator==(const ConstIterator& rhs) const noexcept;
			bool operator!=(const ConstIterator& rhs) const noexcept;
			ConstIterator& operator++();
			ConstIterator operator++(int);

		private:
			const SList* mOwner{ nullptr };
			std::shared_ptr<Node> mNode{ nullptr };
		};
#pragma endregion ConstIterator

	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		SList() = default;

		/// <summary>
		/// Copy constructor.
		/// Takes in a list as a parameters, then copies the data values to the constructed list.
		/// </summary>
		/// <param name="rhs">List to be copied.</param>
		SList(const SList& rhs);

		/// <summary>
		/// Move constructor.
		/// Takes a list as a parameter and moves the data to the constructed list.
		/// </summary>
		/// <param name="rhs"></param>
		SList(SList&& rhs);

		/// <summary>
		/// Assignment operator.
		/// Copies the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">List whose values are copied.</param>
		/// <returns>Modified list with copied values.</returns>
		SList& operator=(const SList& rhs);

		/// <summary>
		/// Assignment operator.
		/// Copies the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">List whose values are copied.</param>
		/// <returns>Modified list with copied values.</returns>
		SList& operator=(SList&& rhs);

		/// <summary>
		/// Class destructor. Clears all existing node references.
		/// </summary>
		~SList();

		/// <summary>
		/// Equals operator. 
		/// Checks if the size of the list and the data values are equal to the size and values of the right hand side (rhs) list.
		/// </summary>
		/// <param name="rhs">The list on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are equivalent, otherwise false.</returns>
		bool operator==(const SList& rhs) const noexcept;

		/// <summary>
		/// Not equal operator. 
		/// Checks if the size of the list and the data values are equal to the size and values of the right hand side (rhs) list.
		/// </summary>
		/// <param name="rhs">The list on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are not equivalent, otherwise false.</returns>
		bool operator!=(const SList& rhs) const noexcept;

		/// <summary>
		/// Getter method for the number of elements in the list.
		/// </summary>
		/// <returns>Number of elements in the list.</returns>
		size_t Size() const;

		/// <summary>
		/// Checks if the size of the list is greater than zero, indicating the list is non-empty.
		/// </summary>
		/// <returns>True if the list contains no elements, otherwise false.</returns>
		bool IsEmpty() const;

		// TODO: Add Documentation
		Iterator begin();
		ConstIterator begin() const;
		ConstIterator cbegin() const;

		/// <summary>
		/// Getter method for the first data value in the list.
		/// </summary>
		/// <returns>Reference to the first data value in the list.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty list.</exception>
		T& Front();

		/// <summary>
		/// Getter method for the first data value in the list, as a constant.
		/// </summary>
		/// <returns>Reference to the first data value in the list, as a constant.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty list.</exception>
		const T& Front() const;

		/// <summary>
		/// Adds an element with the passed in data to the front of the list.
		/// </summary>
		/// <param name="data">A data value to be added to the front of the list.</param>
		void PushFront(const T& data);

		/// <summary>
		/// Removes the first element from the list.
		/// </summary>
		void PopFront();

		/// <summary>
		/// Getter method for the last data value in the list.
		/// </summary>
		/// <returns>Reference to the first data value in the list.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty list.</exception>
		T& Back();

		/// <summary>
		/// Getter method for the lase data value in the list, as a constant.
		/// </summary>
		/// <returns>Reference to the last data value in the list, as a constant.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty list.</exception>
		const T& Back() const;

		/// <summary>
		/// Adds an element with the passed in data to the back of the list.
		/// </summary>
		/// <param name="data">A data value to be added to the back of the list.</param>
		void PushBack(const T& data);

		/// <summary>
		/// Removes the last element from the list.
		/// </summary>
		void PopBack();

		// TODO: Add Documentation
		Iterator end();
		ConstIterator end() const;
		ConstIterator cend() const;

		Iterator Find(const T& value);
		ConstIterator Find(const T& value) const;
		Iterator InsertAfter(const T& data, const Iterator& iterator);
		bool Remove(const Iterator& iterator);

		/// <summary>
		/// Removes all elements from the list and resets the size to zero.
		/// </summary>
		void Clear();

	private:
		/// <summary>
		/// Number of elements in the list.
		/// </summary>
		size_t mSize{ 0 };

		/// <summary>
		/// First element in the list.
		/// </summary>
		std::shared_ptr<Node> mFront{ nullptr };

		/// <summary>
		/// Last element in the list.
		/// </summary>
		std::shared_ptr<Node> mBack{ nullptr };
	};
}

#include "SList.inl"