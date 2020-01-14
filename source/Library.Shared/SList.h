#pragma once

#include <memory>
#include <initializer_list>
#include <stdexcept>
#include <functional>
#include <algorithm>

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
		/// <summary>
		/// Class for traversing the list and retrieving values, which can then be manipulated.
		/// </summary>
		class Iterator final
		{
			friend SList;
			friend class ConstIterator;

		private:
			/// <summary>
			/// Specialized constructor for creating an iterator for a list at a given node.
			/// </summary>
			/// <param name="list">Source list for the iterator's values.</param>
			/// <param name="node">Current element of the list referenced by the iterator.</param>
			Iterator(const SList<T>& list, std::shared_ptr<Node> node=nullptr);

		public:
			/* Iterator Traits */
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::forward_iterator_tag;

			/* Defaults */
			Iterator() = default;
			Iterator(const Iterator& rhs) = default;
			Iterator& operator=(const Iterator& rhs) = default;
			Iterator(Iterator&& rhs) = default;
			Iterator& operator=(Iterator&& rhs) = default;
			~Iterator() = default;

		public:
			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>Value of the current element of the list.</returns>
			/// <exception cref="runtime_error">Attempted to access an iterator with no associated value.</exception>
			T& operator*() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner list and element are equal to the left, false otherwise.</returns>
			bool operator==(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Not equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner list and element are inequal to the left, false otherwise.</returns>
			bool operator!=(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Pre-increment operator.
			/// </summary>
			/// <returns>Reference to the next iterator.</returns>
			/// <exception cref="runtime_error">Cannot increment past the end of the list, including when empty.</exception>
			Iterator& operator++();

			/// <summary>
			/// Post-increment operator.
			/// </summary>
			/// <returns>A copy of the iterator before it was incremented.</returns>
			/// <exception cref="runtime_error">Cannot increment past the end of the list, including when empty.</exception>
			Iterator operator++(int);

		private:
			/// <summary>
			/// Owner list that is able to be traversed by the iterator instance.
			/// </summary>
			const SList* mOwner{ nullptr };

			/// <summary>
			/// Node reference that contains the current element referenced by the iterator instance.
			/// </summary>
			std::shared_ptr<Node> mNode{ nullptr };
		};
#pragma endregion Iterator

#pragma region ConstIterator
		/// <summary>
		/// Class for traversing the list and reading values, may not manipulate the list.
		/// </summary>
		class ConstIterator final
		{
			friend SList;

		public:
			/// <summary>
			/// Specialized copy constructor that enables the construction of a ConstIterator from a non-const Itrerator.
			/// </summary>
			/// <param name="iterator"></param>
			ConstIterator(const Iterator& iterator);

		private:
			/// <summary>
			/// Specialized constructor for creating an iterator for a list at a given node.
			/// </summary>
			/// <param name="list">Source list for the iterator's values.</param>
			/// <param name="node">Current element of the list referenced by the iterator, defaulted to a nullptr value.</param>
			ConstIterator(const SList& list, std::shared_ptr<Node> node=nullptr);

		public:
			/* Iterator Traits */
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = const T*;
			using reference = const T&;
			using iterator_category = std::forward_iterator_tag;

			/* Defaults */
			ConstIterator() = default;
			ConstIterator(const ConstIterator&) = default;
			ConstIterator& operator=(const ConstIterator&) = default;
			ConstIterator(ConstIterator&&) = default;
			ConstIterator& operator=(ConstIterator&&) = default;
			~ConstIterator() = default;

		public:
			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>Value of the current element of the list.</returns>
			/// <exception cref="runtime_error">Attempted to access an iterator with no associated value.</exception>
			const T& operator*() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner list and element are equal to the left, false otherwise.</returns>
			bool operator==(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner list and element are inequal to the left, false otherwise.</returns>
			bool operator!=(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Pre-increment operator.
			/// </summary>
			/// <returns>Reference to the next iterator.</returns>
			/// <exception cref="runtime_error">Cannot increment past the end of the list, including when empty.</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Post-increment operator.
			/// </summary>
			/// <returns>A copy of the iterator before it was incremented.</returns>
			/// <exception cref="runtime_error">Cannot increment past the end of the list, including when empty.</exception>
			ConstIterator operator++(int);

		private:
			/// <summary>
			/// Owner list that is able to be traversed by the iterator instance.
			/// </summary>
			const SList* mOwner{ nullptr };

			/// <summary>
			/// Node reference that contains the current element referenced by the iterator instance.
			/// </summary>
			std::shared_ptr<Node> mNode{ nullptr };
		};
#pragma endregion ConstIterator

	public:
		/* Iterator Traits */
		using value_type = T;

		/// <summary>
		/// Default constructor.
		/// </summary>
		SList() = default;

		/// <summary>
		/// Initializer list constructor.
		/// </summary>
		/// <param name="list">Value list for initializing a new SList.</param>
		SList(const std::initializer_list<T> rhs);

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
		/// <param name="rhs">List to be moved.</param>
		SList(SList&& rhs) noexcept;

		/// <summary>
		/// Assignment to initializer list constructor.
		/// </summary>
		/// <param name="list">Value list for initializing a new SList.</param>
		SList& operator=(const std::initializer_list<T> rhs);

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
		SList& operator=(SList&& rhs) noexcept;

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

		/// <summary>
		/// Gets an iterator pointing to the first element in the list, values are mutable.
		/// </summary>
		/// <returns>Iterator to the first element in the list.</returns>
		Iterator begin();

		/// <summary>
		/// Gets an iterator pointing to the first element in the list, values are immutable.
		/// </summary>
		/// <returns>Constant value iterator to the first element in the list.</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets an iterator pointing to the first element in the list, values are immutable.
		/// </summary>
		/// <returns>Constant value iterator to the first element in the list.</returns>
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
		/// <returns>Iterator pointing to the newly added node.</returns>
		Iterator PushFront(const T& data);

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
		/// <returns>Iterator pointing to the newly added node.</returns>
		Iterator PushBack(const T& data);

		/// <summary>
		/// Removes the last element from the list.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Gets an iterator pointing past the last element in the list, value is mutable.
		/// </summary>
		/// <returns>Iterator to the last element in the list.</returns>	
		Iterator end();

		/// <summary>
		/// Gets an iterator pointing past the last element in the list, value is immutable.
		/// </summary>
		/// <returns>Constant value iterator to the last element in the list.</returns>	
		ConstIterator end() const;

		/// <summary>
		/// Gets an iterator pointing past the last element in the list, value is immutable.
		/// </summary>
		/// <returns>Constant value iterator to the last element in the list.</returns>	
		ConstIterator cend() const;

		/// <summary>
		/// Searches the list for a given value and returns an iterator.
		/// </summary>
		/// <param name="value">Value to search for in the list.</param>
		/// <param name="equal">Equality functor for comparing the search value to elements in the list.</param>
		/// <returns>An iterator referencing the value, if found. Otherwise it returns an empty iterator.</returns>
		Iterator Find(const T& value, std::function<bool(T, T)> equal=[](T a, T b) { return a == b; });
		
		/// <summary>
		/// Searches the list for a given value and returns an iterator.
		/// </summary>
		/// <param name="value">Value to search for in the list.</param>
		/// <param name="equal">Equality functor for comparing the search value to elements in the list.</param>
		/// <returns>An const value iterator referencing the value, if found. Otherwise it returns an empty iterator.</returns>
		ConstIterator Find(const T& value, std::function<bool(T, T)> equal=[](T a, T b) { return a == b; }) const;

		/// <summary>
		/// Inserts a new element in the list in between a given iterator and the following element.
		/// </summary>
		/// <param name="iterator">An iterator used to insert a new element in the following position.</param>
		/// <param name="data">A value to be used to create a new node.</param>
		/// <returns>An iterator referencing the new node.</returns>
		/// <exception cref="runtime_error">Iterator not associated with this list.</exception>
		Iterator InsertAfter(const Iterator& iterator, const T& data);

		/// <summary>
		/// Removes a single element from the list given the corresponding iterator.
		/// </summary>
		/// <param name="vale">Value to be searched for in the list to be removed.</param>
		/// <param name="equal">Equality functor for comparing the search value to elements in the list.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		/// <exception cref="runtime_error">Iterator not associated with this list.</exception>
		bool Remove(const T& value, std::function<bool(T, T)> equal=[](T a, T b) { return a == b; });

		/// <summary>
		/// Removes a single element from the list given the corresponding iterator.
		/// </summary>
		/// <param name="iterator">An iterator referencing the element in the list to be removed.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		/// <exception cref="runtime_error">Iterator not associated with this list.</exception>
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