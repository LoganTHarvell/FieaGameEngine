#pragma once

#include <memory>
#include <initializer_list>
#include <stdexcept>
#include <functional>
#include <algorithm>

namespace Library
{
	/// <summary>
	/// Represents a generic vector list.
	/// </summary>
	template <typename T>
	class Vector
	{
	public:
		/* Iterator Traits */
		using value_type = T;

	private:
		/* Helper Data Structures/Functors */

#pragma region Iterator
		/// <summary>
		/// Class for traversing the vector and retrieving values, which can then be manipulated.
		/// </summary>
		class Iterator final
		{
			friend Vector;
			friend class ConstIterator;

		public:
			/* Iterator Traits */
			using size_type = size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::forward_iterator_tag;

		private:
			/// <summary>
			/// Specialized constructor for creating an iterator for a vector at a given node.
			/// </summary>
			/// <param name="vector">Source vector for the iterator's values.</param>
			/// <param name="node">Current element of the vector referenced by the iterator.</param>
			Iterator(const Vector<T>& vector, size_t index=0);

		public:
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
			/// <returns>Value of the current element of the vector.</returns>
			/// <exception cref="runtime_error">Attempted to access an iterator with no associated value.</exception>
			T& operator*() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner vector and element are equal to the left, false otherwise.</returns>
			bool operator==(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Not equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner vector and element are inequal to the left, false otherwise.</returns>
			bool operator!=(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Pre-increment operator.
			/// </summary>
			/// <returns>Reference to the next iterator.</returns>
			/// <exception cref="runtime_error">Cannot increment past the end of the vector, including when empty.</exception>
			Iterator& operator++();

			/// <summary>
			/// Post-increment operator.
			/// </summary>
			/// <returns>A copy of the iterator before it was incremented.</returns>
			/// <exception cref="runtime_error">Cannot increment past the end of the vector, including when empty.</exception>
			Iterator operator++(int);

		private:
			/// <summary>
			/// Owner vector that is able to be traversed by the iterator instance.
			/// </summary>
			const Vector* mOwner{ nullptr };

			/// <summary>
			/// Index of the current element referenced by the iterator instance.
			/// </summary>
			size_t mIndex{ 0 };
		};
#pragma endregion Iterator

#pragma region ConstIterator
		/// <summary>
		/// Class for traversing the vector and reading values, may not manipulate the vector.
		/// </summary>
		class ConstIterator final
		{
			friend Vector;

		public:
			/* Iterator Traits */
			using size_type = size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = const T*;
			using reference = const T&;
			using iterator_category = std::forward_iterator_tag;

		public:
			/// <summary>
			/// Specialized copy constructor that enables the construction of a ConstIterator from a non-const Itrerator.
			/// </summary>
			/// <param name="iterator"></param>
			ConstIterator(const Iterator& iterator);

		private:
			/// <summary>
			/// Specialized constructor for creating an iterator for a vector at a given node.
			/// </summary>
			/// <param name="vector">Source vector for the iterator's values.</param>
			/// <param name="node">Current element of the vector referenced by the iterator, defaulted to a nullptr value.</param>
			ConstIterator(const Vector& vector, size_t index=0);

		public:
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
			/// <returns>Value of the current element of the vector.</returns>
			/// <exception cref="runtime_error">Attempted to access an iterator with no associated value.</exception>
			const T& operator*() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner vector and element are equal to the left, false otherwise.</returns>
			bool operator==(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner vector and element are inequal to the left, false otherwise.</returns>
			bool operator!=(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Pre-increment operator.
			/// </summary>
			/// <returns>Reference to the next iterator.</returns>
			/// <exception cref="runtime_error">Cannot increment past the end of the vector, including when empty.</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Post-increment operator.
			/// </summary>
			/// <returns>A copy of the iterator before it was incremented.</returns>
			/// <exception cref="runtime_error">Cannot increment past the end of the vector, including when empty.</exception>
			ConstIterator operator++(int);

		private:
			/// <summary>
			/// Owner vector that is able to be traversed by the iterator instance.
			/// </summary>
			const Vector* mOwner{ nullptr };

			/// <summary>
			/// Node reference that contains the current element referenced by the iterator instance.
			/// </summary>
			size_t mIndex{ 0 };
		};
#pragma endregion ConstIterator

		/// <summary>
		/// Functor specifying the default strategy for incrementing the capacity of the vector.
		/// </summary>
		struct DefaultReserveStrategy
		{
			/// <summary>
			/// Function call operator.
			/// </summary>
			/// <param name="">Placeholder for the vector size.</param>
			/// <param name="capacity">Vector capacity.</param>
			/// <returns></returns>
			size_t operator()(const size_t, const size_t capacity) const
			{
				return static_cast<size_t>(capacity * 1.5);
			}
		};

	public:
#pragma region Constructors
		/// <summary>
		/// Default constructor.
		/// </summary>
		Vector(const size_t capacity=0);

		/// <summary>
		/// Class destructor. Clears all existing node references.
		/// </summary>
		~Vector();

		/// <summary>
		/// Copy constructor.
		/// Takes in a vector as a parameters, then copies the data values to the constructed vector.
		/// </summary>
		/// <param name="rhs">List to be copied.</param>
		Vector(const Vector& rhs);

		/// <summary>
		/// Move constructor.
		/// Takes a vector as a parameter and moves the data to the constructed vector.
		/// </summary>
		/// <param name="rhs">List to be moved.</param>
		Vector(Vector&& rhs) noexcept;

		/// <summary>
		/// Initializer vector constructor.
		/// </summary>
		/// <param name="rhs">Value list for intializing a new vector.</param>
		Vector(const std::initializer_list<T> rhs);
#pragma endregion Constructors

#pragma region AssignmentOperators
		/// <summary>
		/// Copy assignment operator.
		/// Copies the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">List whose values are copied.</param>
		/// <returns>Modified vector with copied values.</returns>
		Vector& operator=(const Vector& rhs);

		/// <summary>
		/// Move assignment operator.
		/// Moves the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">List whose values are copied.</param>
		/// <returns>Modified vector with copied values.</returns>
		Vector& operator=(Vector&& rhs) noexcept;

		/// <summary>
		/// Initializer list assignment operator.
		/// </summary>
		/// <param name="vector">Value vector for initializing a new Vector.</param>
		Vector& operator=(const std::initializer_list<T> rhs);
#pragma endregion AssignmentOperators

#pragma region BooleanOperators
		/// <summary>
		/// Equals operator. 
		/// Checks if the size of the vector and the data values are equal to the size and values of the right hand side (rhs) vector.
		/// </summary>
		/// <param name="rhs">The vector on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are equivalent, otherwise false.</returns>
		bool operator==(const Vector& rhs) const noexcept;

		/// <summary>
		/// Not equal operator. 
		/// Checks if the size of the vector and the data values are equal to the size and values of the right hand side (rhs) vector.
		/// </summary>
		/// <param name="rhs">The vector on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are not equivalent, otherwise false.</returns>
		bool operator!=(const Vector& rhs) const noexcept;
#pragma endregion BooleanOperators

#pragma region IteratorAccessors
		/// <summary>
		/// Gets an iterator pointing to the first element in the vector, values are mutable.
		/// </summary>
		/// <returns>Iterator to the first element in the vector.</returns>
		Iterator begin();

		/// <summary>
		/// Gets an iterator pointing to the first element in the vector, values are immutable.
		/// </summary>
		/// <returns>Constant value iterator to the first element in the vector.</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets an iterator pointing to the first element in the vector, values are immutable.
		/// </summary>
		/// <returns>Constant value iterator to the first element in the vector.</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets an iterator pointing past the last element in the vector, value is mutable.
		/// </summary>
		/// <returns>Iterator to the last element in the vector.</returns>	
		Iterator end();

		/// <summary>
		/// Gets an iterator pointing past the last element in the vector, value is immutable.
		/// </summary>
		/// <returns>Constant value iterator to the last element in the vector.</returns>	
		ConstIterator end() const;

		/// <summary>
		/// Gets an iterator pointing past the last element in the vector, value is immutable.
		/// </summary>
		/// <returns>Constant value iterator to the last element in the vector.</returns>	
		ConstIterator cend() const;

		/// <summary>
		/// Searches the vector for a given value and returns an iterator.
		/// </summary>
		/// <param name="value">Value to search for in the vector.</param>
		/// <param name="equal">Equality functor for comparing the search value to elements in the vector.</param>
		/// <returns>An iterator referencing the value, if found. Otherwise it returns an empty iterator.</returns>
		Iterator Find(const T& value, std::function<bool(T, T)> equal=[](T a, T b) { return a == b; });

		/// <summary>
		/// Searches the vector for a given value and returns an iterator.
		/// </summary>
		/// <param name="value">Value to search for in the vector.</param>
		/// <param name="equal">Equality functor for comparing the search value to elements in the vector.</param>
		/// <returns>An const value iterator referencing the value, if found. Otherwise it returns an empty iterator.</returns>
		ConstIterator Find(const T& value, std::function<bool(T, T)> equal=[](T a, T b) { return a == b; }) const;
#pragma endregion IteratorAccessors


#pragma region SizeCapacity
		/// <summary>
		/// Getter method for the number of elements in the vector.
		/// </summary>
		/// <returns>Number of initialized elements.</returns>
		size_t Size() const;	
		
		/// <summary>
		/// Checks if the size of the vector is greater than zero, indicating the vector is non-empty.
		/// </summary>
		/// <returns>True if the vector contains no elements, otherwise false.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Getter method for the max number of elements for which memory is allocated.
		/// </summary>
		/// <returns>Max number of elements for which memory is allocated.</returns>
		size_t Capacity() const;

		/// <summary>
		/// Allocates memory for the specified capacity. Preserving any existing elements.
		/// </summary>
		/// <param name="capacity">Max number of elements for which to allocate memory.</param>
		/// <exception cref="runtime_error">Insufficient memory."</exception>
		void Reserve(const size_t capacity);
#pragma endregion SizeCapacity

#pragma region ElementAccessors
		/// <summary>
		/// Retrieves a value reference for the element at the specified index.
		/// </summary>
		/// <param name="index">Position of an element in the vector.</param>
		/// <returns>Reference to the value of the element at the given index.</returns>
		/// <exception cref="runtime_error">Index is out of bounds.</exception>
		T& At(const size_t index);

		/// <summary>
		/// Retrieves a const value reference for the element at the specified index.
		/// </summary>
		/// <param name="index">Position of an element in the vector.</param>
		/// <returns>Const value reference to the value of the element at the given index.</returns>
		/// <exception cref="runtime_error">Index is out of bounds.</exception>
		const T& At(const size_t index) const;

		/// <summary>
		/// Offset dereference operator.
		/// Retrieves a value reference for the element at the specified index.
		/// </summary>
		/// <param name="index">Position of an element in the vector.</param>
		/// <returns>Reference to the value of the element at the given index.</returns>
		/// <exception cref="runtime_error">Index is out of bounds.</exception>
		T& operator[](const size_t index);

		/// <summary>
		/// Offset dereference operator.
		/// Retrieves a const value reference for the element at the specified index.
		/// </summary>
		/// <param name="index">Position of an element in the vector.</param>
		/// <returns>Const value reference to the value of the element at the given index.</returns>
		/// <exception cref="runtime_error">Index is out of bounds.</exception>
		const T& operator[](const size_t index) const;

		/// <summary>
		/// Getter method for the first data value in the vector.
		/// </summary>
		/// <returns>Reference to the first data value in the vector.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty vector.</exception>
		T& Front();

		/// <summary>
		/// Getter method for the first data value in the vector, as a constant.
		/// </summary>
		/// <returns>Reference to the first data value in the vector, as a constant.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty vector.</exception>
		const T& Front() const;

		/// <summary>
		/// Getter method for the last data value in the vector.
		/// </summary>
		/// <returns>Reference to the first data value in the vector.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty vector.</exception>
		T& Back();

		/// <summary>
		/// Getter method for the lase data value in the vector, as a constant.
		/// </summary>
		/// <returns>Reference to the last data value in the vector, as a constant.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty vector.</exception>
		const T& Back() const;
#pragma endregion ElementAccessors

#pragma region Modifiers
		/// <summary>
		/// Adds an element with the passed in data to the back of the vector.
		/// </summary>
		/// <param name="data">A data value to be added to the back of the vector.</param>
		void PushBack(const T& data);

		/// <summary>
		/// Removes the last element from the vector.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Removes a single element from the vector given the corresponding iterator.
		/// </summary>
		/// <param name="vale">Value to be searched for in the vector to be removed.</param>
		/// <param name="equal">Equality functor for comparing the search value to elements in the vector.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		/// <exception cref="runtime_error">Iterator not associated with this vector.</exception>
		bool Remove(const T& value, std::function<bool(T, T)> equal = [](T a, T b) { return a == b; });

		/// <summary>
		/// Removes a single element from the vector given the corresponding iterator.
		/// </summary>
		/// <param name="iterator">An iterator referencing the element in the vector to be removed.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		/// <exception cref="runtime_error">Iterator not associated with this vector.</exception>
		bool Remove(const Iterator& iterator);

		/// <summary>
		/// Removes all elements from the vector and resets the size to zero.
		/// </summary>
		void Clear();
#pragma endregion Modifiers

	private:
		/// <summary>
		/// Pointer to a memory block for inserting elements into the vector.
		/// </summary>
		T* mData{ nullptr };

		/// <summary>
		/// Number of elements in the vector.
		/// </summary>
		size_t mSize{ 0 };

		/// <summary>
		/// Number of elements for which memory is reserved, but not necessarily initialized.
		/// </summary>
		size_t mCapacity{ 0 };

		/// <summary>
		/// Reserve strategy functor.
		/// </summary>
		std::function<size_t(const size_t, const size_t)> mReserveStrategy{ DefaultReserveStrategy() };
	};
}

#include "Vector.inl"