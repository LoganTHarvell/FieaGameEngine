#pragma once

#pragma region Includes
// Standard
#include <initializer_list>
#include <stdexcept>
#include <cassert>
#include <functional>
#include <algorithm>

// First Party
#include "DefaultEquality.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Represents a generic Vector list.
	/// </summary>
	/// <typeparam name="T">Data type of elements in a Vector.</typeparam>
	template <typename T>
	class Vector final
	{
#pragma region Type Definitions
	public:
		/// <summary>
		/// Value type for std::iterator_trait.
		/// </summary>
		using value_type = T;

		/// <summary>
		/// Reserve strategy functor type used during resize on insertion.
		/// </summary>
		using ReserveFunctor = std::function<std::size_t(const std::size_t, const std::size_t)>;
		
		/// <summary>
		/// Equality functor type used to evaluate equality of Vector types.
		/// </summary>
		using EqualityFunctor = std::function<bool(const T&, const T&)>;
#pragma endregion Type Definitions

#pragma region Default Functors
	private:
		/// <summary>
		/// Functor specifying the default strategy for incrementing the capacity of the Vector.
		/// </summary>
		struct DefaultReserveFunctor final
		{
			/// <summary>
			/// Function call operator.
			/// </summary>
			/// <param name="">Placeholder argument for the Vector size.</param>
			/// <param name="capacity">Vector capacity.</param>
			/// <returns>New capacity.</returns>
			constexpr std::size_t operator()(const std::size_t, const std::size_t capacity) const;
		};
#pragma endregion Default Functors

#pragma region Iterator
	public:
		/// <summary>
		/// Class for traversing the Vector and retrieving values, which can then be manipulated.
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
			using iterator_category = std::bidirectional_iterator_tag;
#pragma endregion Iterator Traits

		public:
			/* Defaults */
			Iterator() = default;
			~Iterator() = default;
			Iterator(const Iterator& rhs) = default;
			Iterator& operator=(const Iterator& rhs) = default;
			Iterator(Iterator&& rhs) = default;
			Iterator& operator=(Iterator&& rhs) = default;

		private:
			/// <summary>
			/// Specialized constructor for creating an Iterator for a Vector at a given index.
			/// </summary>
			/// <param name="vector">Source Vector for the Iterator's values.</param>
			/// <param name="index">Current element of the Vector referenced by the Iterator.</param>
			Iterator(Vector& vector, const std::size_t index=0);

		public:
			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>Value of the current element of the Vector.</returns>
			/// <exception cref="runtime_error">Iterator invalid.</exception>
			T& operator*() const;

			/// <summary>
			/// Member access operator.
			/// </summary>
			/// <returns>Pointer to the value of the current element of the Vector.</returns>
			/// <exception cref="runtime_error">Invalid Iterator.</exception>
			T* operator->() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side Iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner Vector and element are equal to the left, false otherwise.</returns>
			bool operator==(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Not equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side Iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner Vector and element are inequal to the left, false otherwise.</returns>
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
			Iterator& operator+=(const std::size_t rhs);

			/// <summary>
			/// Addition operator.
			/// </summary>
			/// <param name="rhs">Value to offset the Iterator.</param>
			/// <returns>Iterator at the given offset from this Iterator.</returns>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			Iterator operator+(const std::size_t rhs) const;

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
			Iterator& operator-=(const std::size_t rhs);

			/// <summary>
			/// Subtraction operator.
			/// </summary>
			/// <param name="rhs">Value to offset the Iterator.</param>
			/// <returns>Iterator at the given offset from this Iterator.</returns>
			/// <exception cref="runtime_error">Iterator invalid.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			Iterator operator-(const std::size_t rhs) const;

			/// <summary>
			/// Subscript operator.
			/// </summary>
			/// <param name="rhs">Value to offset the Iterator.</param>
			/// <returns>Iterator at the given offset from this Iterator.</returns>
			/// <exception cref="runtime_error">Iterator invalid.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			Iterator operator[](const std::size_t rhs) const;

		private:
			/// <summary>
			/// Owner Vector that is able to be traversed by the Iterator instance.
			/// </summary>
			Vector* mOwner{ nullptr };

			/// <summary>
			/// Index of the current element referenced by the Iterator instance.
			/// </summary>
			std::size_t mIndex{ 0 };
		};
#pragma endregion Iterator

#pragma region ConstIterator
	public:
		/// <summary>
		/// Class for traversing the Vector and reading values, may not manipulate the Vector.
		/// </summary>
		class ConstIterator final
		{
			friend Vector;

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
			using iterator_category = std::bidirectional_iterator_tag;
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
			/// <param name="it">ConstIterator to be copied.</param>
			ConstIterator(const Iterator& it);

		private:
			/// <summary>
			/// Specialized constructor for creating an ConstIterator for a Vector at a given index.
			/// </summary>
			/// <param name="vector">Source Vector for the ConstIterator's values.</param>
			/// <param name="index">Current element of the Vector referenced by the ConstIterator, defaulted to a nullptr value.</param>
			ConstIterator(const Vector& vector, const std::size_t index=0);

		public:
			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>Value of the current element of the Vector.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			const T& operator*() const;

			/// <summary>
			/// Member access operator.
			/// </summary>
			/// <returns>Pointer to the value of the current element of the Vector.</returns>
			/// <exception cref="runtime_error">Invalid ConstIterator.</exception>
			const T* operator->() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side ConstIterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner Vector and element are equal to the left, false otherwise.</returns>
			bool operator==(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side ConstIterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner Vector and element are inequal to the left, false otherwise.</returns>
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
			ConstIterator& operator+=(const std::size_t rhs);

			/// <summary>
			/// Addition operator.
			/// </summary>
			/// <param name="rhs">Value to offset the ConstIterator.</param>
			/// <returns>ConstIterator at the given offset from this ConstIterator.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			ConstIterator operator+(const std::size_t rhs) const;

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
			ConstIterator& operator-=(const std::size_t rhs);

			/// <summary>
			/// Subtraction operator.
			/// </summary>
			/// <param name="rhs">Value to offset the ConstIterator.</param>
			/// <returns>ConstIterator at the given offset from this ConstIterator.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			ConstIterator operator-(const std::size_t rhs) const;

			/// <summary>
			/// Subscript operator.
			/// </summary>
			/// <param name="rhs">Value to offset the ConstIterator.</param>
			/// <returns>Iterator at the given offset from this ConstIterator.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			ConstIterator operator[](const std::size_t rhs) const;
		
		private:
			/// <summary>
			/// Owner Vector that is able to be traversed by the ConstIterator instance.
			/// </summary>
			const Vector* mOwner{ nullptr };

			/// <summary>
			/// Node reference that contains the current element referenced by the ConstIterator instance.
			/// </summary>
			std::size_t mIndex{ 0 };
		};
#pragma endregion ConstIterator

#pragma region Constructors, Destructor, Assignment
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="capacity">Default capacity for the vector.</param>
		/// <param name="equalityFunctor">Default equality functor.</param>
		/// <param name="reserveFunctor">Default reserve strategy functor.</param>
		explicit Vector(const std::size_t capacity=0, const EqualityFunctor equalityFunctor=DefaultEquality<T>(), const ReserveFunctor reserveFunctor=DefaultReserveFunctor());

		/// <summary>
		/// Destructor. 
		/// Clears all existing elements.
		/// </summary>
		~Vector();

		/// <summary>
		/// Copy constructor.
		/// Takes in a Vector as a parameter, then copies the data values to the constructed Vector.
		/// </summary>
		/// <param name="rhs">Vector to be copied.</param>
		Vector(const Vector& rhs);

		/// <summary>
		/// Copy assignment operator.
		/// Copies the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">Vector whose values are copied.</param>
		/// <returns>Modified Vector with copied values.</returns>
		Vector& operator=(const Vector& rhs);

		/// <summary>
		/// Move constructor.
		/// Takes a Vector as a parameter and moves the data to the constructed Vector.
		/// </summary>
		/// <param name="rhs">Vector to be moved.</param>
		Vector(Vector&& rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// Moves the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">Vector whose values are copied.</param>
		/// <returns>Modified Vector with copied values.</returns>
		Vector& operator=(Vector&& rhs) noexcept;

		/// <summary>
		/// Initializer list constructor.
		/// </summary>
		/// <param name="rhs">Value list for intializing a new Vector.</param>
		/// <remarks>May require an EqualityFunctor passed using constructor syntax, if no suitable DefaultEquality exists.</remarks>
		/// <param name="reserveStrategy">Default reserve strategy functor.</param>
		/// <param name="equalityFunctor">Default equality functor.</param>
		Vector(const std::initializer_list<T> rhs, const EqualityFunctor equalityFunctor=DefaultEquality<T>(), const ReserveFunctor reserveFunctor=DefaultReserveFunctor());

		/// <summary>
		/// Initializer list assignment operator.
		/// </summary>
		/// <param name="rhs">Value Vector for initializing a new Vector.</param>
		/// <remarks>Does not initialize equality functor. Must call SetEqualityFunctor for full functionality.</remarks>
		Vector& operator=(const std::initializer_list<T> rhs);
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
	public:
		/// <summary>
		/// Equals operator. 
		/// Checks if the size of the Vector and the data values are equal to the size and values of the right hand side (rhs) Vector.
		/// </summary>
		/// <param name="rhs">Vector on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are equivalent, otherwise false.</returns>
		bool operator==(const Vector& rhs) const noexcept;

		/// <summary>
		/// Not equal operator. 
		/// Checks if the size of the Vector and the data values are equal to the size and values of the right hand side (rhs) Vector.
		/// </summary>
		/// <param name="rhs">Vector on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are not equivalent, otherwise false.</returns>
		bool operator!=(const Vector& rhs) const noexcept;
#pragma endregion Boolean Operators

#pragma region Size and Capacity
	public:
		/// <summary>
		/// Getter method for the number of elements in the Vector.
		/// </summary>
		/// <returns>Number of initialized elements.</returns>
		std::size_t Size() const;	
		
		/// <summary>
		/// Checks if the size of the Vector is greater than zero, indicating the Vector is non-empty.
		/// </summary>
		/// <returns>True if the Vector contains no elements, otherwise false.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Getter method for the max number of elements for which memory is allocated.
		/// </summary>
		/// <returns>Max number of elements for which memory is allocated.</returns>
		std::size_t Capacity() const;

		/// <summary>
		/// Allocates memory for the specified capacity. Preserving any existing elements.
		/// </summary>
		/// <param name="capacity">Max number of elements for which to allocate memory.</param>
		/// <exception cref="runtime_error">Failed memory reallocation."</exception>
		void Reserve(const std::size_t capacity);

		/// <summary>
		/// Resizes the content of the Vector to the given size.
		/// Either destructs excess elements or initializes new elements.
		/// </summary>
		/// <param name="size">New size for the Vector, signifying the number of initialized elements.</param>
		void Resize(const std::size_t size);

		/// <summary>
		/// Resizes the content of the Vector to the given size.
		/// Either destructs excess elements or initializes new elements.
		/// </summary>
		/// <param name="size">New size for the Vector, signifying the number of initialized elements.</param>
		/// <param name="value">New value for the Vector, if new elements are initialized.</param>
		void Resize(const std::size_t size, const T& value);

		/// <summary>
		/// Reduces the capacity of the Vector to fit the content size.
		/// </summary>
		/// <exception cref="runtime_error">Failed memory reallocation.</exception>
		void ShrinkToFit();
#pragma endregion Size and Capacity

#pragma region Iterator Accessors
	public:
		/// <summary>
		/// Gets an Iterator pointing to the first element in the Vector, values are mutable.
		/// </summary>
		/// <returns>Iterator to the first element in the Vector.</returns>
		Iterator begin();

		/// <summary>
		/// Gets an ConstIterator pointing to the first element in the Vector, values are immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the first element in the Vector.</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets an ConstIterator pointing to the first element in the Vector, values are immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the first element in the Vector.</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets an Iterator pointing past the last element in the Vector, value is mutable.
		/// </summary>
		/// <returns>Iterator to the last element in the Vector.</returns>	
		Iterator end();

		/// <summary>
		/// Gets an ConstIterator pointing past the last element in the Vector, value is immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the last element in the Vector.</returns>	
		ConstIterator end() const;

		/// <summary>
		/// Gets an ConstIterator pointing past the last element in the Vector, value is immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the last element in the Vector.</returns>	
		ConstIterator cend() const;

		/// <summary>
		/// Searches the Vector for a given value and returns an Iterator.
		/// </summary>
		/// <param name="value">Value to search for in the Vector.</param>
		/// <returns>Iterator referencing the value, if found. Otherwise it returns an Iterator to the end.</returns>
		Iterator Find(const T& value);

		/// <summary>
		/// Searches the Vector for a given value and returns an ConstIterator.
		/// </summary>
		/// <param name="value">Value to search for in the Vector.</param>
		/// <returns>ConstIterator referencing the value, if found. Otherwise it returns an ConstIterator to the end.</returns>
		ConstIterator Find(const T& value) const;
#pragma endregion Iterator Accessors

#pragma region Element Accessors
	public:
		/// <summary>
		/// Getter method for the first data value in the Vector.
		/// </summary>
		/// <returns>Reference to the first data value in the Vector.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty Vector.</exception>
		T& Front();

		/// <summary>
		/// Getter method for the first data value in the Vector, as a constant.
		/// </summary>
		/// <returns>Reference to the first data value in the Vector, as a constant.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty Vector.</exception>
		const T& Front() const;

		/// <summary>
		/// Getter method for the last data value in the Vector.
		/// </summary>
		/// <returns>Reference to the first data value in the Vector.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty Vector.</exception>
		T& Back();

		/// <summary>
		/// Getter method for the lase data value in the Vector, as a constant.
		/// </summary>
		/// <returns>Reference to the last data value in the Vector, as a constant.</returns>
		/// <exception cref="runtime_error">Thrown when called on an empty Vector.</exception>
		const T& Back() const;

		/// <summary>
		/// Retrieves a value reference for the element at the specified index.
		/// </summary>
		/// <param name="index">Position of an element in the Vector.</param>
		/// <returns>Reference to the value of the element at the given index.</returns>
		/// <exception cref="out_of_range">Index is out of bounds.</exception>
		T& At(const std::size_t index);

		/// <summary>
		/// Retrieves a const value reference for the element at the specified index.
		/// </summary>
		/// <param name="index">Position of an element in the Vector.</param>
		/// <returns>Const value reference to the value of the element at the given index.</returns>
		/// <exception cref="out_of_range">Index is out of bounds.</exception>
		const T& At(const std::size_t index) const;

		/// <summary>
		/// Offset dereference operator.
		/// Retrieves a value reference for the element at the specified index.
		/// </summary>
		/// <param name="index">Position of an element in the Vector.</param>
		/// <returns>Reference to the value of the element at the given index.</returns>
		/// <exception cref="out_of_range">Index is out of bounds.</exception>
		T& operator[](const std::size_t index);

		/// <summary>
		/// Offset dereference operator.
		/// Retrieves a const value reference for the element at the specified index.
		/// </summary>
		/// <param name="index">Position of an element in the Vector.</param>
		/// <returns>Const value reference to the value of the element at the given index.</returns>
		/// <exception cref="out_of_range">Index is out of bounds.</exception>
		const T& operator[](const std::size_t index) const;
#pragma endregion Element Accessors

#pragma region Modifiers
	public:
		/// <summary>
		/// Adds an element with the passed in data to the back of the Vector.
		/// </summary>
		/// <param name="data">Value to be added to the back of the Vector.</param>
		void PushBack(const T& data);

		/// <summary>
		/// Removes the last element from the Vector.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Removes a single element from the Vector given the corresponding value.
		/// </summary>
		/// <param name="value">Value to be searched for in the Vector to be removed.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		bool Remove(const T& value);

		/// <summary>
		/// Removes a single element from the Vector given the corresponding Iterator.
		/// </summary>
		/// <param name="it">Iterator referencing the element in the Vector to be removed.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		/// <exception cref="runtime_error">Invalid Iterator.</exception>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Removes all elements from the Vector and resets the size to zero.
		/// </summary>
		void Clear();
#pragma endregion Modifiers

#pragma region Data Members
	private:
		/// <summary>
		/// Pointer to a memory block for inserting elements into the Vector.
		/// </summary>
		T* mIntData{ nullptr };

		/// <summary>
		/// Number of elements in the Vector.
		/// </summary>
		std::size_t mSize{ 0 };

		/// <summary>
		/// Number of elements for which memory is reserved, but not necessarily initialized.
		/// </summary>
		std::size_t mCapacity{ 0 };

		/// <summary>
		/// Functor for evaluating the capacity reserve strategy during resize during element insert.
		/// </summary>
		ReserveFunctor mReserveFunctor{ DefaultReserveFunctor() };

		/// <summary>
		/// Functor for evaluating the equality of two values in the Vector.
		/// </summary>
		EqualityFunctor mEqualityFunctor{ DefaultEquality<T>() };
#pragma endregion Data Members
	};
}

#include "Vector.inl"