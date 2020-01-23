#pragma once

#include <memory>
#include <initializer_list>
#include <stdexcept>
#include <functional>
#include <algorithm>

#include "DefaultEquality.h"

namespace Library
{
	/// <summary>
	/// Represents a generic singly linked list.
	/// </summary>
	template <typename T>
	class SList final
	{
	public:
		/// <summary>
		/// Value type for std::iterator_trait.
		/// </summary>
		using value_type = T;

		/// <summary>
		/// Equality functor type used to evaluate equality of elements in the list.
		/// </summary>
		using EqualityFunctor = std::function<bool(T, T)>;

	private:
#pragma region Node
		/// <summary>
		/// Represents an element in a list.
		/// </summary>
		struct Node final
		{
			T Data;
			std::shared_ptr<Node> Next;
		
			Node(const T& data, std::shared_ptr<Node> next=nullptr);
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

		public:
#pragma region Iterator Traits
			/// <summary>
			/// Size type for std::iterator_trait.
			/// </summary>
			using size_type = std::size_t;

			/// <summary>
			/// Difference type for std::iterator_trait.
			/// </summary>
			using difference_type = std::ptrdiff_t;

			/// <summary>
			/// Value type for std::iterator_trait.
			/// </summary>
			using value_type = T;

			/// <summary>
			/// Pointer type for std::iterator_trait.
			/// </summary>
			using pointer = T*;

			/// <summary>
			/// Reference type for std::iterator_trait.
			/// </summary>
			using reference = T&;

			/// <summary>
			/// Iterator category for std::iterator_trait.
			/// </summary>
			using iterator_category = std::forward_iterator_tag;
#pragma endregion Iterator Traits

			/* Defaults */
			Iterator() = default;
			Iterator(const Iterator& rhs) = default;
			Iterator& operator=(const Iterator& rhs) = default;
			Iterator(Iterator&& rhs) = default;
			Iterator& operator=(Iterator&& rhs) = default;
			~Iterator() = default;

			/// <summary>
			/// Specialized constructor for creating an Iterator for a list at a given node.
			/// </summary>
			/// <param name="list">Source list for the Iterator's values.</param>
			/// <param name="node">Current element of the list referenced by the Iterator.</param>
			Iterator(const SList<T>& owner, std::shared_ptr<Node> node=nullptr);

		public:
			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>Value of the current element of the list.</returns>
			/// <exception cref="runtime_error">Invalid Iterator.</exception>
			T& operator*() const;

			/// <summary>
			/// Member access operator.
			/// </summary>
			/// <returns>Pointer to the value of the current element of the list.</returns>
			/// <exception cref="runtime_error">Invalid Iterator.</exception>
			T* operator->() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side Iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner list and element are equal to the left, false otherwise.</returns>
			bool operator==(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Not equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side Iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner list and element are inequal to the left, false otherwise.</returns>
			bool operator!=(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Pre-increment operator.
			/// </summary>
			/// <returns>Reference to the next Iterator.</returns>
			/// <exception cref="runtime_error">Invalid Iterator.</exception>
			Iterator& operator++();

			/// <summary>
			/// Post-increment operator.
			/// </summary>
			/// <returns>Copy of the Iterator before it was incremented.</returns>
			Iterator operator++(int);

		private:
			/// <summary>
			/// Owner list that is able to be traversed by the Iterator instance.
			/// </summary>
			const SList* mOwner{ nullptr };

			/// <summary>
			/// Node reference that contains the current element referenced by the Iterator instance.
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
#pragma region Iterator Traits
			/// <summary>
			/// Size type for std::iterator_trait.
			/// </summary>
			using size_type = std::size_t;

			/// <summary>
			/// Difference type for std::iterator_trait.
			/// </summary>
			using difference_type = std::ptrdiff_t;

			/// <summary>
			/// Value type for std::iterator_trait.
			/// </summary>
			using value_type = T;

			/// <summary>
			/// Pointer type for std::iterator_trait.
			/// </summary>
			using pointer = const T*;

			/// <summary>
			/// Reference type for std::iterator_trait.
			/// </summary>
			using reference = const T&;

			/// <summary>
			/// Iterator category for std::iterator_trait.
			/// </summary>
			using iterator_category = std::forward_iterator_tag;
#pragma endregion Iterator Traits

		public:
			/* Defaults */
			ConstIterator() = default;
			ConstIterator(const ConstIterator&) = default;
			ConstIterator& operator=(const ConstIterator&) = default;
			ConstIterator(ConstIterator&&) = default;
			ConstIterator& operator=(ConstIterator&&) = default;
			~ConstIterator() = default;

			/// <summary>
			/// Specialized copy constructor that enables the construction of a ConstIterator from a non-const Itrerator.
			/// </summary>
			/// <param name="it"></param>
			ConstIterator(const Iterator& it);

		private:
			/// <summary>
			/// Specialized constructor for creating an ConstIterator for a list at a given node.
			/// </summary>
			/// <param name="list">Source list for the ConstIterator's values.</param>
			/// <param name="node">Current element of the list referenced by the ConstIterator, defaulted to a nullptr value.</param>
			ConstIterator(const SList& owner, std::shared_ptr<Node> node=nullptr);

		public:
			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>Value of the current element of the list.</returns>
			/// <exception cref="runtime_error">Invalid ConstIterator.</exception>
			const T& operator*() const;

			/// <summary>
			/// Member access operator.
			/// </summary>
			/// <returns>Pointer to the value of the current element of the list.</returns>
			/// <exception cref="runtime_error">Invalid ConstIterator.</exception>
			const T* operator->() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side ConstIterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner list and element are equal to the left, false otherwise.</returns>
			bool operator==(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side ConstIterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner list and element are inequal to the left, false otherwise.</returns>
			bool operator!=(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Pre-increment operator.
			/// </summary>
			/// <returns>Reference to the next ConstIterator.</returns>
			/// <exception cref="runtime_error">Invalid ConstIterator.</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Post-increment operator.
			/// </summary>
			/// <returns>Copy of the ConstIterator before it was incremented.</returns>
			ConstIterator operator++(int);

		private:
			/// <summary>
			/// Owner list that is able to be traversed by the ConstIterator instance.
			/// </summary>
			const SList* mOwner{ nullptr };

			/// <summary>
			/// Node reference that contains the current element referenced by the ConstIterator instance.
			/// </summary>
			std::shared_ptr<Node> mNode{ nullptr };
		};
#pragma endregion ConstIterator

	public:
#pragma region Constructors and Destructor
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="equalityFunctor">Equality functor for comparing list elements.</param>
		/// <remarks>If no EqualityFunctor is passed and no valid DefaultEquality exists, then a compiler error will occur.</remarks>
		SList(const EqualityFunctor equalityFunctor=DefaultEquality<T>());

		/// <summary>
		/// Destructor. 
		/// Clears all existing node references.
		/// </summary>
		~SList();

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
		/// Initializer list constructor.
		/// </summary>
		/// <param name="list">Value list for initializing a new SList.</param>
		/// <remarks>Does not initialize equality functor. User should follow with a call to SetEqualityFunctor.</remarks>
		SList(const std::initializer_list<T> rhs);
#pragma endregion Constructors and Destructor

#pragma region Assignment Operators
		/// <summary>
		/// Copy assignment operator.
		/// Copies the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">List whose values are copied.</param>
		/// <returns>Modified list with copied values.</returns>
		SList& operator=(const SList& rhs);

		/// <summary>
		/// Move assignment operator.
		/// Copies the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">List whose values are copied.</param>
		/// <returns>Modified list with copied values.</returns>
		SList& operator=(SList&& rhs) noexcept;

		/// <summary>
		/// Initializer list assignment operator.
		/// </summary>
		/// <param name="list">Value list for initializing a new SList.</param>
		/// <remarks>Does not initialize equality functor. User should follow with a call to SetEqualityFunctor.</remarks>
		SList& operator=(const std::initializer_list<T> rhs);
#pragma endregion Assignment Operators

#pragma region Boolean Operators
		/// <summary>
		/// Equals operator. 
		/// Checks if the size of the list and the data values are equal to the size and values of the right hand side (rhs) list.
		/// </summary>
		/// <param name="rhs">List on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are equivalent, otherwise false.</returns>
		bool operator==(const SList& rhs) const noexcept;

		/// <summary>
		/// Not equal operator. 
		/// Checks if the size of the list and the data values are equal to the size and values of the right hand side (rhs) list.
		/// </summary>
		/// <param name="rhs">List on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are not equivalent, otherwise false.</returns>
		bool operator!=(const SList& rhs) const noexcept;
#pragma endregion Boolean Operators

#pragma region Size and Capacity
		/// <summary>
		/// Getter method for the number of elements in the list.
		/// </summary>
		/// <returns>Number of elements in the list.</returns>
		std::size_t Size() const;

		/// <summary>
		/// Checks if the size of the list is greater than zero, indicating the list is non-empty.
		/// </summary>
		/// <returns>True if the list contains no elements, otherwise false.</returns>
		bool IsEmpty() const;
#pragma endregion Size

#pragma region Iterator Accessors
		/// <summary>
		/// Gets an Iterator pointing to the first element in the list, values are mutable.
		/// </summary>
		/// <returns>Iterator to the first element in the list.</returns>
		Iterator begin();

		/// <summary>
		/// Gets an ConstIterator pointing to the first element in the list, values are immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the first element in the list.</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets an ConstIterator pointing to the first element in the list, values are immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the first element in the list.</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets an Iterator pointing past the last element in the list, value is mutable.
		/// </summary>
		/// <returns>Iterator to the last element in the list.</returns>	
		Iterator end();

		/// <summary>
		/// Gets an ConstIterator pointing past the last element in the list, value is immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the last element in the list.</returns>	
		ConstIterator end() const;

		/// <summary>
		/// Gets an ConstIterator pointing past the last element in the list, value is immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the last element in the list.</returns>	
		ConstIterator cend() const;

		/// <summary>
		/// Searches the list for a given value and returns an Iterator.
		/// </summary>
		/// <param name="value">Value to search for in the list.</param>
		/// <returns>Iterator referencing the value, if found. Otherwise it returns an empty Iterator.</returns>
		/// <exception cref="runtime_error">Missing equality functor.</exception>
		Iterator Find(const T& value);

		/// <summary>
		/// Searches the list for a given value and returns an ConstIterator.
		/// </summary>
		/// <param name="value">Value to search for in the list.</param>
		/// <returns>const value ConstIterator referencing the value, if found. Otherwise it returns an empty ConstIterator.</returns>
		/// <exception cref="runtime_error">Missing equality functor.</exception>
		ConstIterator Find(const T& value) const;
#pragma endregion Iterator Accessors

#pragma region Element Accessors
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
#pragma endregion Element Accessors

#pragma region Modifiers
		/// <summary>
		/// Adds an element with the passed in data to the front of the list.
		/// </summary>
		/// <param name="data">Value to be added to the front of the list.</param>
		void PushFront(const T& data);

		/// <summary>
		/// Adds an element with the passed in data to the back of the list.
		/// </summary>
		/// <param name="data">Value to be added to the back of the list.</param>
		void PushBack(const T& data);

		/// <summary>
		/// Inserts a new element in the list in between a given Iterator and the following element.
		/// </summary>
		/// <param name="it">Iterator used to insert a new element in the following position.</param>
		/// <param name="data">Value to be used to create a new node.</param>
		/// <returns>Iterator referencing the new node.</returns>
		/// <exception cref="runtime_error">Invalid Iterator.</exception>
		Iterator InsertAfter(const Iterator& it, const T& data);

		/// <summary>
		/// Removes the first element from the list.
		/// </summary>
		void PopFront();

		/// <summary>
		/// Removes the last element from the list.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Removes a single element from the list given the corresponding Iterator.
		/// </summary>
		/// <param name="vale">Value to be searched for in the list to be removed.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		/// <exception cref="runtime_error">Missing equality functor.</exception>
		/// <exception cref="runtime_error">Invalid Iterator.</exception>
		bool Remove(const T& value);

		/// <summary>
		/// Removes a single element from the list given the corresponding Iterator.
		/// </summary>
		/// <param name="it">Iterator referencing the element in the list to be removed.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		/// <exception cref="runtime_error">Invalid Iterator.</exception>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Removes all elements from the list and resets the size to zero.
		/// </summary>
		void Clear();

		/// <summary>
		/// Setter for the equality functor used to compare elements in the list.
		/// </summary>
		/// <param name="equalityFunctor">New equality functor to be used.</param>
		void SetEqualityFunctor(const EqualityFunctor equalityFunctor);
#pragma endregion Modifiers

	private:
		/// <summary>
		/// Number of elements in the list.
		/// </summary>
		std::size_t mSize{ 0 };

		/// <summary>
		/// First element in the list.
		/// </summary>
		std::shared_ptr<Node> mFront{ nullptr };

		/// <summary>
		/// Last element in the list.
		/// </summary>
		std::shared_ptr<Node> mBack{ nullptr };

		/// <summary>
		/// Functor for evaluating the equality of two values in the list.
		/// </summary>
		EqualityFunctor mEqualityFunctor;
	};
}

#include "SList.inl"