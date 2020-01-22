#pragma once

#include <memory>
#include <initializer_list>
#include <stdexcept>
#include <cassert>
#include <functional>
#include <algorithm>

#include "DefaultEquality.h"

namespace Library
{
	/// <summary>
	/// Represents a generic vector list.
	/// </summary>
	template <typename T>
	class Vector final
	{
	public:
		/// <summary>
		/// Value type for std::iterator_trait.
		/// </summary>
		using value_type = T;

		/// <summary>
		/// Reserve strategy functor type used during resize on insertion.
		/// </summary>
		using ReserveStrategy = std::function<size_t(const size_t, const size_t)>;
		
		/// <summary>
		/// Equality functor type used to evaluate equality of vector types.
		/// </summary>
		using EqualityFunctor = std::function<bool(T, T)>;

	public:
		/// <summary>
		/// Functor specifying the default strategy for incrementing the capacity of the vector.
		/// </summary>
		struct DefaultReserveStrategy final
		{
			/// <summary>
			/// Function call operator.
			/// </summary>
			/// <param name="">Placeholder for the vector size.</param>
			/// <param name="capacity">Vector capacity.</param>
			/// <returns>New capacity.</returns>
			const size_t operator()(const size_t, const size_t capacity) const
			{
				return static_cast<size_t>(capacity * 1.5);
			}
		};

#pragma region Iterator
		/// <summary>
		/// Class for traversing the vector and retrieving values, which can then be manipulated.
		/// </summary>
		class Iterator final
		{
			friend Vector;
			friend class ConstIterator;

		public:
#pragma region Iterator Traits
			/// <summary>
			/// Size type for std::iterator_trait.
			/// </summary>
			using size_type = size_t;
			
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
			Iterator(const Iterator& rhs) = default;
			Iterator& operator=(const Iterator& rhs) = default;
			Iterator(Iterator&& rhs) = default;
			Iterator& operator=(Iterator&& rhs) = default;
			~Iterator() = default;

		private:
			/// <summary>
			/// Specialized constructor for creating an Iterator for a vector at a given index.
			/// </summary>
			/// <param name="vector">Source vector for the Iterator's values.</param>
			/// <param name="index">Current element of the vector referenced by the Iterator.</param>
			Iterator(Vector& vector, const size_t index=0);

		public:
			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>Value of the current element of the vector.</returns>
			/// <exception cref="runtime_error">Iterator invalid.</exception>
			T& operator*() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side Iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner vector and element are equal to the left, false otherwise.</returns>
			bool operator==(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Not equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side Iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner vector and element are inequal to the left, false otherwise.</returns>
			bool operator!=(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Pre-increment operator.
			/// </summary>
			/// <returns>Reference to the next Iterator.</returns>
			/// <exception cref="runtime_error">Iterator invalid.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			Iterator& operator++();

			/// <summary>
			/// Post-increment operator.
			/// </summary>
			/// <returns>Copy of the Iterator before it was incremented.</returns>
			/// <exception cref="runtime_error">Iterator invalid.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			Iterator operator++(int);

			/// <summary>
			/// Addition assignment operator.
			/// </summary>
			/// <param name="rhs">Value to offset the Iterator.</param>
			/// <returns>Reference to the Iterator with the given offset.</returns>
			/// <exception cref="runtime_error">Iterator invalid.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			Iterator& operator+=(const size_t rhs);

			/// <summary>
			/// Addition operator.
			/// </summary>
			/// <param name="rhs">Value to offset the Iterator.</param>
			/// <returns>Iterator at the given offset from this Iterator.</returns>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			Iterator operator+(const size_t rhs);

			/// <summary>
			/// Pre-decrement operator.
			/// </summary>
			/// <returns>Reference to the next Iterator.</returns>
			/// <exception cref="runtime_error">Iterator invalid.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			Iterator& operator--();

			/// <summary>
			/// Post-decrement operator.
			/// </summary>
			/// <returns>Copy of the Iterator before it was decremented.</returns>
			/// <exception cref="runtime_error">Iterator invalid.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>	
			Iterator operator--(int);

			/// <summary>
			/// Subtraction assignment operator.
			/// </summary>
			/// <param name="rhs">Value to offset the Iterator.</param>
			/// <returns>Reference to the Iterator with the given offset.</returns>
			/// <exception cref="runtime_error">Iterator invalid.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			Iterator& operator-=(const size_t rhs);

			/// <summary>
			/// Subtraction operator.
			/// </summary>
			/// <param name="rhs">Value to offset the Iterator.</param>
			/// <returns>Iterator at the given offset from this Iterator.</returns>
			/// <exception cref="runtime_error">Iterator invalid.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			Iterator operator-(const size_t rhs);

		private:
			/// <summary>
			/// Owner vector that is able to be traversed by the Iterator instance.
			/// </summary>
			Vector* mOwner{ nullptr };

			/// <summary>
			/// Index of the current element referenced by the Iterator instance.
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
#pragma region Iterator Traits
			/// <summary>
			/// Size type for std::iterator_trait.
			/// </summary>
			using size_type = size_t;

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
			/// <param name="it">ConstIterator to be copied.</param>
			ConstIterator(const Iterator& it);

		private:
			/// <summary>
			/// Specialized constructor for creating an ConstIterator for a vector at a given index.
			/// </summary>
			/// <param name="vector">Source vector for the ConstIterator's values.</param>
			/// <param name="index">Current element of the vector referenced by the ConstIterator, defaulted to a nullptr value.</param>
			ConstIterator(const Vector& vector, size_t index=0);

		public:
			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>Value of the current element of the vector.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			const T& operator*() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side ConstIterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner vector and element are equal to the left, false otherwise.</returns>
			bool operator==(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side ConstIterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner vector and element are inequal to the left, false otherwise.</returns>
			bool operator!=(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Pre-increment operator.
			/// </summary>
			/// <returns>Reference to the next ConstIterator.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Post-increment operator.
			/// </summary>
			/// <returns>Copy of the ConstIterator before it was incremented.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			ConstIterator operator++(int);

			/// <summary>
			/// Addition assignment operator.
			/// </summary>
			/// <param name="rhs">Value to offset the ConstIterator.</param>
			/// <returns>Reference to the ConstIterator with the given offset.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			ConstIterator& operator+=(const size_t rhs);

			/// <summary>
			/// Addition operator.
			/// </summary>
			/// <param name="rhs">Value to offset the ConstIterator.</param>
			/// <returns>ConstIterator at the given offset from this ConstIterator.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			ConstIterator operator+(const size_t rhs);

			/// <summary>
			/// Pre-decrement operator.
			/// </summary>
			/// <returns>Reference to the next ConstIterator.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			ConstIterator& operator--();

			/// <summary>
			/// Post-decrement operator.
			/// </summary>
			/// <returns>Vopy of the ConstIterator before it was decremented.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			ConstIterator operator--(int);

			/// <summary>
			/// Subtraction assignment operator.
			/// </summary>
			/// <param name="rhs">Value to offset the ConstIterator.</param>
			/// <returns>Reference to the ConstIterator with the given offset.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			ConstIterator& operator-=(const size_t rhs);

			/// <summary>
			/// Subtraction operator.
			/// </summary>
			/// <param name="rhs">Value to offset the ConstIterator.</param>
			/// <returns>ConstIterator at the given offset from this ConstIterator.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			ConstIterator operator-(const size_t rhs);
		
		private:
			/// <summary>
			/// Owner vector that is able to be traversed by the ConstIterator instance.
			/// </summary>
			const Vector* mOwner{ nullptr };

			/// <summary>
			/// Node reference that contains the current element referenced by the ConstIterator instance.
			/// </summary>
			size_t mIndex{ 0 };
		};
#pragma endregion ConstIterator

	public:
#pragma region Constructor and Destructor
		/// <summary>
		/// Default constructor.
		/// </summary>
		Vector(const size_t capacity=0, const ReserveStrategy reserveStrategy=DefaultReserveStrategy(), const EqualityFunctor equalityFunctor=DefaultEquality<T>());

		/// <summary>
		/// Destructor. 
		/// Clears all existing elements.
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
		/// Initializer list constructor.
		/// </summary>
		/// <param name="rhs">Value list for intializing a new vector.</param>
		/// <remarks>Does not initialize equality functor. User should follow with a call to SetEqualityFunctor.</remarks>
		Vector(const std::initializer_list<T> rhs);
#pragma endregion Constructors and Destructor

#pragma region Assignment Operators
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
		/// <remarks>Does not initialize equality functor. User should follow with a call to SetEqualityFunctor.</remarks>
		Vector& operator=(const std::initializer_list<T> rhs);
#pragma endregion Assignment Operators

#pragma region Boolean Operators
		/// <summary>
		/// Equals operator. 
		/// Checks if the size of the vector and the data values are equal to the size and values of the right hand side (rhs) vector.
		/// </summary>
		/// <param name="rhs">Vector on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are equivalent, otherwise false.</returns>
		bool operator==(const Vector& rhs) const noexcept;

		/// <summary>
		/// Not equal operator. 
		/// Checks if the size of the vector and the data values are equal to the size and values of the right hand side (rhs) vector.
		/// </summary>
		/// <param name="rhs">Vector on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are not equivalent, otherwise false.</returns>
		bool operator!=(const Vector& rhs) const noexcept;
#pragma endregion Boolean Operators

#pragma region Iterator Accessors
		/// <summary>
		/// Gets an Iterator pointing to the first element in the vector, values are mutable.
		/// </summary>
		/// <returns>Iterator to the first element in the vector.</returns>
		Iterator begin();

		/// <summary>
		/// Gets an ConstIterator pointing to the first element in the vector, values are immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the first element in the vector.</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets an ConstIterator pointing to the first element in the vector, values are immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the first element in the vector.</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets an Iterator pointing past the last element in the vector, value is mutable.
		/// </summary>
		/// <returns>Iterator to the last element in the vector.</returns>	
		Iterator end();

		/// <summary>
		/// Gets an ConstIterator pointing past the last element in the vector, value is immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the last element in the vector.</returns>	
		ConstIterator end() const;

		/// <summary>
		/// Gets an ConstIterator pointing past the last element in the vector, value is immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the last element in the vector.</returns>	
		ConstIterator cend() const;

		/// <summary>
		/// Searches the vector for a given value and returns an Iterator.
		/// </summary>
		/// <param name="value">Value to search for in the vector.</param>
		/// <returns>Iterator referencing the value, if found. Otherwise it returns an empty Iterator.</returns>
		/// <exception cref="">Missing equality functor.</exception>
		Iterator Find(const T& value);

		/// <summary>
		/// Searches the vector for a given value and returns an ConstIterator.
		/// </summary>
		/// <param name="value">Value to search for in the vector.</param>
		/// <returns>ConstIterator referencing the value, if found. Otherwise it returns an empty ConstIterator.</returns>
		/// <exception cref="">Missing equality functor.</exception>
		ConstIterator Find(const T& value) const;
#pragma endregion Iterator Accessors

#pragma region Size and Capacity
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
		/// <exception cref="runtime_error">Failed memory reallocation."</exception>
		void Reserve(const size_t capacity);

		/// <summary>
		/// Resizes the content of the vector to the given size.
		/// Either destructs excess elements or initializes new elements.
		/// </summary>
		/// <param name="size">New size for the vector, signifying the number of initialized elements.</param>
		void Resize(const size_t size);

		/// <summary>
		/// Resizes the content of the vector to the given size.
		/// Either destructs excess elements or initializes new elements.
		/// </summary>
		/// <param name="size">New size for the vector, signifying the number of initialized elements.</param>
		/// <param name="value">New value for the vector, if new elements are initialized.</param>
		void Resize(const size_t size, const T& value);

		/// <summary>
		/// Reduces the capacity of the vector to fit the content size.
		/// </summary>
		/// <exception cref="runtime_error">Failed memory reallocation.</exception>
		void ShrinkToFit();
#pragma endregion Size and Capacity

#pragma region Element Accessors
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
#pragma endregion Element Accessors

#pragma region Modifiers
		/// <summary>
		/// Adds an element with the passed in data to the back of the vector.
		/// </summary>
		/// <param name="data">Value to be added to the back of the vector.</param>
		void PushBack(const T& data);

		/// <summary>
		/// Removes the last element from the vector.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Removes a single element from the vector given the corresponding Iterator.
		/// </summary>
		/// <param name="value">Value to be searched for in the vector to be removed.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		/// <exception cref="">Missing equality functor.</exception>
		bool Remove(const T& value);

		/// <summary>
		/// Removes a single element from the vector given the corresponding Iterator.
		/// </summary>
		/// <param name="it">Iterator referencing the element in the vector to be removed.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		/// <exception cref="runtime_error">Invalid Iterator.</exception>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Removes all elements from the vector and resets the size to zero.
		/// </summary>
		void Clear();

		/// <summary>
		/// Sets reserve strategy functor for incrementing capacity during insertion.
		/// </summary>
		/// <param name="reserveStrategy">New reserve strategy functor.</param>
		void SetReserveStrategy(const ReserveStrategy reserveStrategy);

		/// <summary>
		/// Sets equality functor used to compare elements in the vector.
		/// </summary>
		/// <param name="equalityFunctor">New equality functor.</param>
		void SetEqualityFunctor(const EqualityFunctor equalityFunctor);
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
		/// Functor for evaluating the capacity reserve strategy during resize during element insert.
		/// </summary>
		ReserveStrategy mReserveStrategy{ DefaultReserveStrategy() };

		/// <summary>
		/// Functor for evaluating the equality of two values in the list.
		/// </summary>
		EqualityFunctor mEqualityFunctor;
	};
}

#include "Vector.inl"