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
	/// <typeparam name="T">Data type of elements in a SList.</typeparam>
	template <typename T>
	class SList final
	{
#pragma region Type Definitions
	public:
		/// <summary>
		/// Value type for std::iterator_trait.
		/// </summary>
		using value_type = T;

		/// <summary>
		/// Equality functor type used to evaluate equality of elements in the SList.
		/// </summary>
		using EqualityFunctor = std::function<bool(const T&, const T&)>;
#pragma endregion Type Definitions

#pragma region Node
	private:
		/// <summary>
		/// Represents an element in a SList.
		/// </summary>
		struct Node final
		{
			T Data;
			std::shared_ptr<Node> Next;
		
			Node(const T& data, const std::shared_ptr<Node> next=nullptr);
		};
#pragma endregion Node

#pragma region Iterator
	public:
		/// <summary>
		/// Class for traversing the SList and retrieving values, which can then be manipulated.
		/// </summary>
		class Iterator final
		{
			friend SList;
			friend class ConstIterator;

#pragma region Iterator Traits
		public:
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

		public:
			/* Defaults */
			Iterator() = default;
			~Iterator() = default;
			Iterator(const Iterator& rhs) = default;
			Iterator& operator=(const Iterator& rhs) = default;
			Iterator(Iterator&& rhs) = default;
			Iterator& operator=(Iterator&& rhs) = default;

			/// <summary>
			/// Specialized constructor for creating an Iterator for a SList at a given node.
			/// </summary>
			/// <param name="list">Source SList for the Iterator's values.</param>
			/// <param name="node">Current element of the SList referenced by the Iterator.</param>
			Iterator(const SList<T>& owner, const std::shared_ptr<Node> node=nullptr);

		public:
			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>Value of the current element of the SList.</returns>
			/// <exception cref="runtime_error">Invalid Iterator.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			T& operator*() const;

			/// <summary>
			/// Member access operator.
			/// </summary>
			/// <returns>Pointer to the value of the current element of the SList.</returns>
			/// <exception cref="runtime_error">Invalid Iterator.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			T* operator->() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side Iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner SList and element are equal to the left, false otherwise.</returns>
			bool operator==(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Not equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side Iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner SList and element are inequal to the left, false otherwise.</returns>
			bool operator!=(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Pre-increment operator.
			/// </summary>
			/// <returns>Reference to the next Iterator.</returns>
			/// <exception cref="runtime_error">Invalid Iterator.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			Iterator& operator++();

			/// <summary>
			/// Post-increment operator.
			/// </summary>
			/// <returns>Copy of the Iterator before it was incremented.</returns>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			Iterator operator++(int);

		private:
			/// <summary>
			/// Owner SList that is able to be traversed by the Iterator instance.
			/// </summary>
			const SList* mOwner{ nullptr };

			/// <summary>
			/// Node reference that contains the current element referenced by the Iterator instance.
			/// </summary>
			std::shared_ptr<Node> mNode{ nullptr };
		};
#pragma endregion Iterator

#pragma region ConstIterator
	public:
		/// <summary>
		/// Class for traversing the SList and reading values, may not manipulate the SList.
		/// </summary>
		class ConstIterator final
		{
			friend SList;

#pragma region Iterator Traits
		public:
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
			~ConstIterator() = default;
			ConstIterator(const ConstIterator&) = default;
			ConstIterator& operator=(const ConstIterator&) = default;
			ConstIterator(ConstIterator&&) = default;
			ConstIterator& operator=(ConstIterator&&) = default;

			/// <summary>
			/// Specialized copy constructor that enables the construction of a ConstIterator from a non-const Iterator.
			/// </summary>
			/// <param name="it"></param>
			ConstIterator(const Iterator& it);

		private:
			/// <summary>
			/// Specialized constructor for creating an ConstIterator for a SList at a given node.
			/// </summary>
			/// <param name="list">Source SList for the ConstIterator's values.</param>
			/// <param name="node">Current element of the SList referenced by the ConstIterator, defaulted to a nullptr value.</param>
			ConstIterator(const SList& owner, const std::shared_ptr<Node> node=nullptr);

		public:
			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>Value of the current element of the SList.</returns>
			/// <exception cref="runtime_error">Invalid ConstIterator.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			const T& operator*() const;

			/// <summary>
			/// Member access operator.
			/// </summary>
			/// <returns>Pointer to the value of the current element of the SList.</returns>
			/// <exception cref="runtime_error">Invalid ConstIterator.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			const T* operator->() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side ConstIterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner SList and element are equal to the left, false otherwise.</returns>
			bool operator==(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side ConstIterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner SList and element are inequal to the left, false otherwise.</returns>
			bool operator!=(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Pre-increment operator.
			/// </summary>
			/// <returns>Reference to the next ConstIterator.</returns>
			/// <exception cref="runtime_error">Invalid ConstIterator.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Post-increment operator.
			/// </summary>
			/// <returns>Copy of the ConstIterator before it was incremented.</returns>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			ConstIterator operator++(int);

		private:
			/// <summary>
			/// Owner SList that is able to be traversed by the ConstIterator instance.
			/// </summary>
			const SList* mOwner{ nullptr };

			/// <summary>
			/// Node reference that contains the current element referenced by the ConstIterator instance.
			/// </summary>
			std::shared_ptr<Node> mNode{ nullptr };
		};
#pragma endregion ConstIterator

#pragma region Constructors, Destructor, Assignment
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="equalityFunctor">Equality functor for comparing SList elements.</param>
		/// <remarks>If no EqualityFunctor is passed and no valid DefaultEquality exists, then a compiler error will occur.</remarks>
		SList(const EqualityFunctor equalityFunctor=DefaultEquality<T>());

		/// <summary>
		/// Destructor. 
		/// Clears all existing node references.
		/// </summary>
		~SList();

		/// <summary>
		/// Copy constructor.
		/// Takes in a SList as a parameter, then copies the data values to the constructed SList.
		/// </summary>
		/// <param name="rhs">SList to be copied.</param>
		SList(const SList& rhs);

		/// <summary>
		/// Copy assignment operator.
		/// Copies the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">SList whose values are copied.</param>
		/// <returns>Modified SList with copied values.</returns>
		SList& operator=(const SList& rhs);

		/// <summary>
		/// Move constructor.
		/// Takes a SList as a parameter and moves the data to the constructed SList.
		/// </summary>
		/// <param name="rhs">SList to be moved.</param>
		SList(SList&& rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// Copies the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">SList whose values are copied.</param>
		/// <returns>Modified SList with copied values.</returns>
		SList& operator=(SList&& rhs) noexcept;

		/// <summary>
		/// Initializer SList constructor.
		/// </summary>
		/// <param name="rhs">Value SList for initializing a new SList.</param>
		/// <param name="equalityFunctor">Equality functor for comparing SList elements.</param>
		/// <remarks>May require an EqualityFunctor passed using constructor syntax, if no suitable DefaultEquality exists.</remarks>
		SList(const std::initializer_list<T> rhs, const EqualityFunctor equalityFunctor=DefaultEquality<T>());

		/// <summary>
		/// Initializer SList assignment operator.
		/// </summary>
		/// <param name="rhs">Value list for initializing a new SList.</param>
		SList& operator=(const std::initializer_list<T> rhs);
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
	public:
		/// <summary>
		/// Equals operator. 
		/// Checks if the size of the SList and the data values are equal to the size and values of the right hand side (rhs) SList.
		/// </summary>
		/// <param name="rhs">SList on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are equivalent, otherwise false.</returns>
		bool operator==(const SList& rhs) const noexcept;

		/// <summary>
		/// Not equal operator. 
		/// Checks if the size of the SList and the data values are equal to the size and values of the right hand side (rhs) SList.
		/// </summary>
		/// <param name="rhs">SList on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are not equivalent, otherwise false.</returns>
		bool operator!=(const SList& rhs) const noexcept;
#pragma endregion Boolean Operators

#pragma region Size and Capacity
	public:
		/// <summary>
		/// Getter method for the number of elements in the SList.
		/// </summary>
		/// <returns>Number of elements in the SList.</returns>
		std::size_t Size() const;

		/// <summary>
		/// Checks if the size of the SList is greater than zero, indicating the SList is non-empty.
		/// </summary>
		/// <returns>True if the SList contains no elements, otherwise false.</returns>
		bool IsEmpty() const;
#pragma endregion Size

#pragma region Iterator Accessors
	public:
		/// <summary>
		/// Gets an Iterator pointing to the first element in the SList, values are mutable.
		/// </summary>
		/// <returns>Iterator to the first element in the SList.</returns>
		Iterator begin();

		/// <summary>
		/// Gets an ConstIterator pointing to the first element in the SList, values are immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the first element in the SList.</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets an ConstIterator pointing to the first element in the SList, values are immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the first element in the SList.</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets an Iterator pointing past the last element in the SList, value is mutable.
		/// </summary>
		/// <returns>Iterator to the last element in the SList.</returns>	
		Iterator end();

		/// <summary>
		/// Gets an ConstIterator pointing past the last element in the SList, value is immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the last element in the SList.</returns>	
		ConstIterator end() const;

		/// <summary>
		/// Gets an ConstIterator pointing past the last element in the SList, value is immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the last element in the SList.</returns>	
		ConstIterator cend() const;

		/// <summary>
		/// Searches the SList for a given value and returns an Iterator.
		/// </summary>
		/// <param name="value">Value to search for in the SList.</param>
		/// <returns>Iterator referencing the value, if found. Otherwise it returns an empty Iterator.</returns>
		Iterator Find(const T& value);

		/// <summary>
		/// Searches the SList for a given value and returns an ConstIterator.
		/// </summary>
		/// <param name="value">Value to search for in the SList.</param>
		/// <returns>const value ConstIterator referencing the value, if found. Otherwise it returns an empty ConstIterator.</returns>
		ConstIterator Find(const T& value) const;
#pragma endregion Iterator Accessors

#pragma region Element Accessors
	public:
		/// <summary>
		/// Getter method for the first data value in the SList.
		/// </summary>
		/// <returns>Reference to the first data value in the SList.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty SList.</exception>
		T& Front();

		/// <summary>
		/// Getter method for the first data value in the SList, as a constant.
		/// </summary>
		/// <returns>Reference to the first data value in the SList, as a constant.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty SList.</exception>
		const T& Front() const;

		/// <summary>
		/// Getter method for the last data value in the SList.
		/// </summary>
		/// <returns>Reference to the first data value in the SList.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty SList.</exception>
		T& Back();

		/// <summary>
		/// Getter method for the lase data value in the SList, as a constant.
		/// </summary>
		/// <returns>Reference to the last data value in the SList, as a constant.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty SList.</exception>
		const T& Back() const;
#pragma endregion Element Accessors

#pragma region Modifiers
	public:
		/// <summary>
		/// Adds an element with the passed in data to the front of the SList.
		/// </summary>
		/// <param name="data">Value to be added to the front of the SList.</param>
		void PushFront(const T& data);

		/// <summary>
		/// Adds an element with the passed in data to the back of the SList.
		/// </summary>
		/// <param name="data">Value to be added to the back of the SList.</param>
		void PushBack(const T& data);

		/// <summary>
		/// Inserts a new element in the SList in between a given Iterator and the following element.
		/// </summary>
		/// <param name="it">Iterator used to insert a new element in the following position.</param>
		/// <param name="data">Value to be used to create a new node.</param>
		/// <returns>Iterator referencing the new node.</returns>
		/// <exception cref="runtime_error">Invalid Iterator.</exception>
		Iterator InsertAfter(const Iterator& it, const T& data);

		/// <summary>
		/// Removes the first element from the SList.
		/// </summary>
		void PopFront();

		/// <summary>
		/// Removes the last element from the SList.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Removes a single element from the SList given the corresponding Iterator.
		/// </summary>
		/// <param name="vale">Value to be searched for in the SList to be removed.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		/// <exception cref="runtime_error">Missing equality functor.</exception>
		/// <exception cref="runtime_error">Invalid Iterator.</exception>
		bool Remove(const T& value);

		/// <summary>
		/// Removes a single element from the SList given the corresponding Iterator.
		/// </summary>
		/// <param name="it">Iterator referencing the element in the SList to be removed.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		/// <exception cref="runtime_error">Invalid Iterator.</exception>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Removes all elements from the SList and resets the size to zero.
		/// </summary>
		void Clear();
#pragma endregion Modifiers

#pragma region Data Members
	private:
		/// <summary>
		/// Number of elements in the SList.
		/// </summary>
		std::size_t mSize{ 0 };

		/// <summary>
		/// First element in the SList.
		/// </summary>
		std::shared_ptr<Node> mFront{ nullptr };

		/// <summary>
		/// Last element in the SList.
		/// </summary>
		std::shared_ptr<Node> mBack{ nullptr };

		/// <summary>
		/// Functor for evaluating the equality of two values in the SList.
		/// </summary>
		EqualityFunctor mEqualityFunctor{ DefaultEquality<T>() };
#pragma endregion Data Members
	};
}

#include "SList.inl"