#pragma once

#include "SList.h"
#include "Vector.h"
#include "DefaultHash.h"

namespace Library
{
	/// <summary>
	/// Represents a HashMap collection of key-value pairs.
	/// </summary>
	/// <typeparam name="TKey">Key value type associated with a TData value in a HashMap.</typeparam>
	/// <typeparam name="TData">Data value type associated with a TKey value in a HashMap.</typeparam>
	template<typename TKey, typename TData>
	class HashMap final
	{
	public:
#pragma region Type Definitions and Constants
		/// <summary>
		/// Pairs of TKey and TData values that make up the elements of the HashMap.
		/// </summary>
		using PairType = std::pair<const TKey, TData>;

		/// <summary>
		/// SList containing PairType values. Contains PairTypes that have hash collisions.
		/// </summary>
		using ChainType = SList<PairType>;

		/// <summary>
		/// Iterator for HashMap chains.
		/// </summary>
		using ChainIterator = typename ChainType::Iterator;

		/// <summary>
		/// Iterator for HashMap chains.
		/// </summary>
		using ChainConstIterator = typename ChainType::ConstIterator;

		/// <summary>
		/// Vector of ChainType elements hashed to an index, termed a bucket.
		/// </summary>
		using BucketType = Vector<ChainType>;

		/// <summary>
		/// Iterator for HashMap buckets.
		/// </summary>
		using BucketIterator = typename BucketType::Iterator;

		/// <summary>
		/// Iterator for HashMap buckets.
		/// </summary>
		using BucketConstIterator = typename BucketType::ConstIterator;

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
		/// Default number of buckets in the HashMap.
		/// </summary>
		static constexpr std::size_t DefaultBucketCount = 31;
#pragma endregion Type Definitions and Constants

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
			~Iterator() = default;
			Iterator(const Iterator& rhs) = default;
			Iterator& operator=(const Iterator& rhs) = default;
			Iterator(Iterator&& rhs) = default;
			Iterator& operator=(Iterator&& rhs) = default;

		private:
			/// <summary>
			/// Specialized constructor for creating an Iterator for a HashMap.
			/// </summary>
			/// <param name="HashMap">Source HashMap for the Iterator's values.</param>
			/// <param name="bucketIterator">BucketIterator pointing to the target ChainType value.</param>
			/// <param name="chainIterator">ChainIterator to the target PairType value.</param>
			Iterator(HashMap& hashMap, const typename BucketIterator& bucketIterator=BucketIterator(), const typename ChainIterator& chainIterator=ChainIterator());

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
			/// <returns>Pointer to the value of the current element of the HashMap.</returns>
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
			BucketIterator mBucketIterator;

			/// <summary>
			/// Chain iterator pointing to the current PairType in the chain of a bucket.
			/// </summary>
			ChainIterator mChainIterator;
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
			/// Specialized constructor for creating an ConstIterator for a HashMap.
			/// </summary>
			/// <param name="HashMap">Source HashMap for the ConstIterator's values.</param>
			/// <param name="bucketIterator">BucketIterator pointing to the target ChainType value.</param>
			/// <param name="chainIterator">ChainIterator to the target PairType value.</param>
			ConstIterator(const HashMap& hashMap, const typename BucketConstIterator& bucketIterator=BucketConstIterator(), const typename ChainConstIterator& chainIterator=ChainConstIterator());

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
			/// <returns>Pointer to the value of the current element of the HashMap.</returns>
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
			BucketConstIterator mBucketIterator;

			/// <summary>
			/// Chain iterator pointing to the current PairType in the chain of a bucket.
			/// </summary>
			ChainConstIterator mChainIterator;
		};
#pragma endregion ConstIterator

	public:
#pragma region Constructors, Destructor, Assignment
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="bucketCount">Number of buckets to initialize for the HashMap. Cannot be zero.</param>
		/// <param name="keyEqualityFunctor">Equality functor for comparing TKey values.</param>
		/// <param name="hashFunctor">Hashing functor for creating hash codes from TKey values.</param>
		/// <remarks cref="bucketCount">Asserts on zero bucketCount.</remarks>
		explicit HashMap(const std::size_t bucketCount=DefaultBucketCount, const KeyEqualityFunctor keyEqualityFunctor=DefaultEquality<TKey>(), const HashFunctor hashFunctor=DefaultHash<TKey>());

		/// <summary>
		/// Default Destructor. 
		/// Clears all existing elements.
		/// </summary>
		~HashMap() = default;

		/// <summary>
		/// Copy constructor.
		/// Takes in a HashMap as a parameter, then copies the data values to the constructed HashMap.
		/// </summary>
		/// <param name="rhs">HashMap to be copied.</param>
		HashMap(const HashMap& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// Copies the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">HashMap whose values are copied.</param>
		/// <returns>Modified HashMap with copied values.</returns>
		HashMap& operator=(const HashMap& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// Takes a HashMap as a parameter and moves the data to the constructed HashMap.
		/// </summary>
		/// <param name="rhs">HashMap to be moved.</param>
		HashMap(HashMap&& rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// Moves the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">HashMap whose values are copied.</param>
		/// <returns>Modified HashMap with copied values.</returns>
		HashMap& operator=(HashMap && rhs) noexcept;

		/// <summary>
		/// Initializer list constructor.
		/// </summary>
		/// <param name="rhs">List of PairType values for insertion.</param>
		/// <param name="bucketCount">Number of buckets to initialize for the HashMap. Cannot be zero.</param>
		/// <param name="keyEqualityFunctor">Equality functor for comparing TKey values.</param>
		/// <param name="hashFunctor">Hashing functor for creating hash codes from TKey values.</param>
		/// <remarks cref="bucketCount">Asserts on zero bucketCount.</remarks>
		HashMap(const std::initializer_list<PairType> rhs, const std::size_t bucketCount = DefaultBucketCount, KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>(), const HashFunctor hashFunctor = DefaultHash<TKey>());

		/// <summary>
		/// Initializer list assignment operator.
		/// </summary>
		/// <param name="rhs">List of values to be in the HashMap.</param>
		/// <returns>Reference to the modified HashMap containing the new pairs.</returns>
		HashMap& operator=(const std::initializer_list<PairType> rhs);
#pragma endregion Constructors, Destructor, and Assignment

#pragma region Size and Capacity
		/// <summary>
		/// Getter method for the number of PairType values in the HashMap.
		/// </summary>
		/// <returns>Number of PairType values in the HashMap.</returns>
		std::size_t Size() const;

		/// <summary>
		/// Getter method for the number of buckets in the HashMap.
		/// </summary>
		/// <returns>Number of buckets in the HashMap.</returns>
		std::size_t BucketCount() const;

		/// <summary>
		/// Checks if the size of the HashMap is greater than zero, indicating it is non-empty.
		/// </summary>
		/// <returns>True if the HashMap contains no elements, otherwise false.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Checks the ratio of the size of the HashMap to the bucket count.
		/// </summary>
		/// <returns>True if the HashMap contains no elements, otherwise false.</returns>
		float LoadFactor() const;

		/// <summary>
		/// Resizes the HashMap to a given bucket count, re-indexing the elements.
		/// </summary>
		/// <param name="bucketCount">New bucket count for the HashMap.</param>
		void Rehash(const std::size_t bucketCount);
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
		/// <param name="key">TKey value to search for in the HashMap.</param>
		/// <returns>Iterator referencing the value, if found. Otherwise it returns an Iterator to the end.</returns>
		Iterator Find(const TKey& key);

		/// <summary>
		/// Searches the HashMap for a given value and returns an ConstIterator.
		/// </summary>
		/// <param name="key">TKey value to search for in the HashMap.</param>
		/// <returns>constIterator referencing the value, if found. Otherwise it returns a ConstIterator to the end.</returns>
		ConstIterator Find(const TKey& key) const;
#pragma endregion Iterator Accessors

#pragma region Element Accessors
		/// <summary>
		/// Retrieves a value reference for the element with the specified key.
		/// </summary>
		/// <param name="key">Key of an element in the HashMap.</param>
		/// <returns>Reference to the value of the element at the given index.</returns>
		/// <exception cref="runtime_error">TKey not found.</exception>
		TData& At(const TKey& key);

		/// <summary>
		/// Retrieves a const value reference for the element with the specified key.
		/// </summary>
		/// <param name="key">Key of an element in the HashMap.</param>
		/// <returns>Const value reference to the value of the element at the given index.</returns>
		/// <exception cref="runtime_error">TKey not found.</exception>
		const TData& At(const TKey& key) const;

		/// <summary>
		/// Subscript operator.
		/// Retrieves a value reference for the element with the specified key.
		/// </summary>
		/// <param name="key">Key of an element in the HashMap.</param>
		/// <returns>Reference to the value of the element at the given index.</returns>
		TData& operator[](const TKey& key);

		/// <summary>
		/// Subscript operator.
		/// Retrieves a const value reference for the element with the specified key.
		/// </summary>
		/// <param name="key">Key of an element in the HashMap.</param>
		/// <returns>Const value reference to the value of the element at the given index.</returns>
		/// <exception cref="runtime_error">TKey not found.</exception>
		const TData& operator[](const TKey& key) const;

		/// <summary>
		/// Checks if a PairType value with the given key is within the HashMap.
		/// </summary>
		/// <param name="key">Key of an element in the HashMap.</param>
		/// <returns>True if the key exists in the HashMap, false otherwise.</returns>
		bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Checks if a PairType value with the given key is within the HashMap.
		/// </summary>
		/// <param name="key">Key of an element in the HashMap.</param>
		/// <param name="dataOut">Reference to be written with the associated TData value, if the key is found.</param>
		/// <returns>True if the key exists in the HashMap, false otherwise.</returns>
		bool ContainsKey(const TKey& key, TData& dataOut);
#pragma endregion Element Accessors

#pragma region Modifiers
		/// <summary>
		/// Attempts to insert a PairType
		/// </summary>
		/// <param name="entry">PairType element to be inserted.</param>
		/// <returns>As a pair, an Iterator to the entry matching the TKey value and a boolean indicating successful insertion.</returns>
		std::pair<Iterator, bool> Insert(const PairType& entry);

		/// <summary>
		/// Removes a single PairType value from the HashMap given the corresponding TKey value.
		/// </summary>
		/// <param name="key">TKey value to be searched for in the HashMap to be removed.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		bool Remove(const TKey& key);

		/// <summary>
		/// Removes a single PairType value from the HashMap given the corresponding TKey value.
		/// </summary>
		/// <param name="it">Iterator pointing to the PairType value to be removed.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Removes all elements from the HashMap and resets the size to zero.
		/// </summary>
		void Clear();
#pragma endregion Modifiers

	private:
#pragma region Helper Methods
		/// <summary>
		/// Searches the HashMap for a given value and returns an Iterator.
		/// </summary>
		/// <param name="key">TKey value to search for in the HashMap.</param>
		/// <param name="indexOut">Reference to be written with the hashed index.</param>
		/// <returns>Iterator referencing the value, if found. Otherwise it returns an Iterator to the end.</returns>
		Iterator Find(const TKey& key, std::size_t& indexOut);
#pragma endregion Helper Methods

	private:
#pragma region Data Members
		/// <summary>
		/// HashMap of chain lists for storing key-value pairs according at a hashed index.
		/// </summary>
		BucketType mBuckets;

		/// <summary>
		/// Number of elements in the HashMap.
		/// </summary>
		std::size_t mSize{ 0 };

		/// <summary>
		/// Equality functor for comparing two TKey values.
		/// </summary>
		KeyEqualityFunctor mKeyEqualityFunctor;

		/// <summary>
		/// Hash functor used to compute hash code from a TKey.
		/// </summary>
		HashFunctor mHashFunctor;
#pragma endregion Data Members
	};
}

#include "HashMap.inl"