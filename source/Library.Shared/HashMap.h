#pragma once

#include "HashMap.h"
#include "SList.h"
#include "DefaultHash.h"

namespace Library
{
	/// <summary>
	/// Represents a collection of key-value pairs.
	/// </summary>
	template<typename TKey, typename TData>
	class HashMap final
	{
	public:
#pragma region Type Definitions
		/// <summary>
		/// Key value pair type that make up the elements of the HashMap.
		/// </summary>
		using PairType = std::pair<const TKey, TData>;

		/// <summary>
		/// Chain list type for containing key value pairs that have collisions.
		/// </summary>
		using ChainType = SList<PairType>;

		/// <summary>
		/// Bucket type that contains a chain list of keys with collisions.
		/// </summary>
		using BucketType = Vector<ChainType>;

		/// <summary>
		/// Value type for std::iterator_trait.
		/// </summary>
		using value_type = PairType;

		/// <summary>
		/// Hash functor type for computing hash codes from a TKey value.
		/// </summary>
		using HashFunctor = std::function<std::size_t(const TKey& key)>;

		/// <summary>
		/// Equality functor type for comparing TKey values.
		/// </summary>
		using KeyEqualityFunctor = std::function<bool(const TKey& lhs, const TKey& rhs)>;

		/// <summary>
		/// Equality functor type for comparing TData values.
		/// </summary>
		using DataEqualityFunctor = std::function<bool(const TData& lhs, const TData& rhs)>;
#pragma endregion Type Definitions

	public:
#pragma region Iterator
		/// <summary>
		/// Class for traversing the HashMap and retrieving values, which can then be manipulated.
		/// </summary>
		class Iterator final
		{
			friend HashMap;
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
			using value_type = PairType;

			/// <summary>
			/// Pointer type for std::iterator_trait.
			/// </summary>
			using pointer = PairType*;

			/// <summary>
			/// Reference type for std::iterator_trait.
			/// </summary>
			using reference = PairType&;

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
			/// Specialized constructor for creating an Iterator for a HashMap.
			/// </summary>
			/// <param name="HashMap">Source HashMap for the Iterator's values.</param>
			/// <param name="bucketIterator">BucketType::Iterator pointing to the target ChainType value.</param>
			/// <param name="chainIterator">ChainType::Iterator to the target PairType value.</param>
			Iterator(HashMap& hashMap, const typename BucketType::Iterator& bucketIterator=BucketType::Iterator(), const typename ChainType::Iterator& chainIterator=ChainType::Iterator());

		public:
			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>Value of the current element of the HashMap.</returns>
			/// <exception cref="runtime_error">Iterator invalid.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			PairType& operator*() const;

			/// <summary>
			/// Member access operator.
			/// </summary>
			/// <returns>Pointer to the value of the current element of the list.</returns>
			/// <exception cref="runtime_error">Invalid Iterator.</exception>
			/// <exception cref="out_of_range">Iterator out of bounds.</exception>
			PairType* operator->() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side Iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner HashMap and element are equal to the left, false otherwise.</returns>
			bool operator==(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Not equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side Iterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner HashMap and element are inequal to the left, false otherwise.</returns>
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

		private:
			/// <summary>
			/// Owner HashMap that is able to be traversed by the Iterator instance.
			/// </summary>
			HashMap* mOwner{ nullptr };

			/// <summary>
			/// Bucket iterator pointing to the current bucket with a chain of PairType entries.
			/// </summary>
			typename BucketType::Iterator mBucketIterator;

			/// <summary>
			/// Chain iterator pointing to the current PairType in the chain of a bucket.
			/// </summary>
			typename ChainType::Iterator mChainIterator;
		};
#pragma endregion Iterator

#pragma region ConstIterator
		/// <summary>
		/// Class for traversing the HashMap and reading values, may not manipulate the HashMap.
		/// </summary>
		class ConstIterator final
		{
			friend HashMap;

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
			using value_type = PairType;

			/// <summary>
			/// Pointer type for std::iterator_trait.
			/// </summary>
			using pointer = const PairType*;

			/// <summary>
			/// Reference type for std::iterator_trait.
			/// </summary>
			using reference = const PairType&;

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
			/// Specialized copy constructor that enables the construction of a ConstIterator from a non-const Iterator.
			/// </summary>
			/// <param name="it">ConstIterator to be copied.</param>
			ConstIterator(const Iterator& it);

		private:
			/// <summary>
			/// Specialized constructor for creating an ConstIterator for a HashMap.
			/// </summary>
			/// <param name="HashMap">Source HashMap for the ConstIterator's values.</param>
			/// <param name="bucketIterator">BucketType::Iterator pointing to the target ChainType value.</param>
			/// <param name="chainIterator">ChainType::Iterator to the target PairType value.</param>
			ConstIterator(const HashMap& hashMap, const typename BucketType::Iterator& bucketIterator=BucketType::Iterator(), const typename ChainType::Iterator& chainIterator=ChainType::Iterator());

		public:
			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>Value of the current element of the HashMap.</returns>
			/// <exception cref="runtime_error">ConstIterator invalid.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			const PairType& operator*() const;

			/// <summary>
			/// Member access operator.
			/// </summary>
			/// <returns>Pointer to the value of the current element of the list.</returns>
			/// <exception cref="runtime_error">Invalid ConstIterator.</exception>
			/// <exception cref="out_of_range">ConstIterator out of bounds.</exception>
			const PairType* operator->() const;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side ConstIterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner HashMap and element are equal to the left, false otherwise.</returns>
			bool operator==(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side ConstIterator to be compared against for equality.</param>
			/// <returns>True when the rhs owner HashMap and element are inequal to the left, false otherwise.</returns>
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

		private:
			/// <summary>
			/// Owner HashMap that is able to be traversed by the ConstIterator instance.
			/// </summary>
			const HashMap* mOwner{ nullptr };

			/// <summary>
			/// Bucket iterator pointing to the current bucket with a chain of PairType entries.
			/// </summary>
			typename BucketType::Iterator mBucketIterator;

			/// <summary>
			/// Chain iterator pointing to the current PairType in the chain of a bucket.
			/// </summary>
			typename ChainType::Iterator mChainIterator;
		};
#pragma endregion ConstIterator

	public:
#pragma region Constructor and Destructor
		/// <summary>
		/// Default constructor.
		/// </summary>
		HashMap(const std::size_t capacity=0, const KeyEqualityFunctor keyEqualityFunctor=DefaultEquality<TKey>(), const DataEqualityFunctor dataEqualityFunctor=DefaultEquality<TData>(), const HashFunctor hashFunctor=DefaultHash<TKey>());

		/// <summary>
		/// Default Destructor. 
		/// Clears all existing elements.
		/// </summary>
		~HashMap() = default;

		/// <summary>
		/// Copy constructor.
		/// Takes in a HashMap as a parameters, then copies the data values to the constructed HashMap.
		/// </summary>
		/// <param name="rhs">List to be copied.</param>
		HashMap(const HashMap& rhs);

		/// <summary>
		/// Move constructor.
		/// Takes a HashMap as a parameter and moves the data to the constructed HashMap.
		/// </summary>
		/// <param name="rhs">List to be moved.</param>
		HashMap(HashMap&& rhs) noexcept;
#pragma endregion Constructors and Destructor

#pragma region Assignment Operators
		/// <summary>
		/// Copy assignment operator.
		/// Copies the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">List whose values are copied.</param>
		/// <returns>Modified HashMap with copied values.</returns>
		HashMap& operator=(const HashMap& rhs);

		/// <summary>
		/// Move assignment operator.
		/// Moves the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">List whose values are copied.</param>
		/// <returns>Modified HashMap with copied values.</returns>
		HashMap& operator=(HashMap&& rhs) noexcept;
#pragma endregion Assignment Operators

#pragma region Size and Capacity
		/// <summary>
		/// Getter method for the number of PairType values in the HashMap.
		/// </summary>
		/// <returns>Number of PairType values in the HashMap.</returns>
		std::size_t Size() const;

		/// <summary>
		/// Checks if the size of the HashMap is greater than zero, indicating it is non-empty.
		/// </summary>
		/// <returns>True if the HashMap contains no elements, otherwise false.</returns>
		bool IsEmpty() const;
#pragma endregion Size and Capacity

#pragma region Iterator Accessors
		/// <summary>
		/// Gets an Iterator pointing to the first element in the HashMap, values are mutable.
		/// </summary>
		/// <returns>Iterator to the first element in the HashMap.</returns>
		Iterator begin();

		/// <summary>
		/// Gets an ConstIterator pointing to the first element in the HashMap, values are immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the first element in the HashMap.</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets an ConstIterator pointing to the first element in the HashMap, values are immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the first element in the HashMap.</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets an Iterator pointing past the last element in the HashMap, value is mutable.
		/// </summary>
		/// <returns>Iterator to the last element in the HashMap.</returns>	
		Iterator end();

		/// <summary>
		/// Gets an ConstIterator pointing past the last element in the HashMap, value is immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the last element in the HashMap.</returns>	
		ConstIterator end() const;

		/// <summary>
		/// Gets an ConstIterator pointing past the last element in the HashMap, value is immutable.
		/// </summary>
		/// <returns>Constant value ConstIterator to the last element in the HashMap.</returns>	
		ConstIterator cend() const;

		/// <summary>
		/// Searches the HashMap for a given value and returns an Iterator.
		/// </summary>
		/// <param name="value">Value to search for in the HashMap.</param>
		/// <returns>Iterator referencing the value, if found. Otherwise it returns an empty Iterator.</returns>
		/// <exception cref="">Missing equality functor.</exception>
		Iterator Find(const TKey& value);

		/// <summary>
		/// Searches the HashMap for a given value and returns an ConstIterator.
		/// </summary>
		/// <param name="value">Value to search for in the HashMap.</param>
		/// <returns>ConstIterator referencing the value, if found. Otherwise it returns an empty ConstIterator.</returns>
		/// <exception cref="">Missing equality functor.</exception>
		ConstIterator Find(const TKey& value) const;
#pragma endregion Iterator Accessors

#pragma region Modifiers
		// TODO: Insert
		std::pair<Iterator, bool> Insert(const PairType& entry);
#pragma endregion Modifiers

	private:
		/// <summary>
		/// HashMap of chain lists for storing key-value pairs according at a hashed index.
		/// </summary>
		Vector<ChainType> mBuckets;

		/// <summary>
		/// Number of elements in the HashMap.
		/// </summary>
		std::size_t mSize{ 0 };

		/// <summary>
		/// Equality functor for comparing two TKey values.
		/// </summary>
		KeyEqualityFunctor mKeyEqualityFunctor;

		/// <summary>
		/// Equality functor for comparing two TData values.
		/// </summary>
		DataEqualityFunctor mDataEqualityFunctor;

		/// <summary>
		/// Hash functor used to compute hash code from a TKey.
		/// </summary>
		HashFunctor mHashFunctor{ DefaultHash<TKey>() };
	};
}

#include "HashMap.inl"