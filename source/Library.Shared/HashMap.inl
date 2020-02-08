#pragma once

#include "HashMap.h"

namespace Library
{
#pragma region Iterator
	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::Iterator::Iterator(HashMap& hashMap, const typename BucketIterator& bucketIterator, const typename ChainIterator& chainIterator) :
		mOwner(&hashMap), mBucketIterator(bucketIterator), mChainIterator(chainIterator)
	{
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::PairType& HashMap<TKey, TData>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}

		return *mChainIterator;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::PairType* HashMap<TKey, TData>::Iterator::operator->() const
	{
		return &(this->operator*());
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Iterator::operator==(const Iterator& rhs) const noexcept
	{
		return !(operator!=(rhs));
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Iterator::operator!=(const Iterator& rhs) const noexcept
	{
		return (mOwner != rhs.mOwner || mChainIterator != rhs.mChainIterator);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator& HashMap<TKey, TData>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}

		if (mBucketIterator == mOwner->mBuckets.end())
		{
			throw std::out_of_range("Iterator out of bounds.");
		}

		if (mChainIterator != mBucketIterator->end())
		{
			++mChainIterator;
		}

		if (mChainIterator == mBucketIterator->end())
		{
			while (++mBucketIterator != mOwner->mBuckets.end() && mBucketIterator->IsEmpty());

			if (mBucketIterator != mOwner->mBuckets.end())
			{
				mChainIterator = mBucketIterator->begin();
			}
			else
			{
				mChainIterator = (mBucketIterator - 1)->end();
			}
		}

		return *this;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Iterator::operator++(int)
	{
		Iterator it = Iterator(*this);
		++(*this);
		return it;
	}
#pragma endregion Iterator

#pragma region ConstIterator
	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator::ConstIterator(const Iterator& it) :
		mOwner(it.mOwner), mBucketIterator(it.mBucketIterator), mChainIterator(it.mChainIterator)
	{
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator::ConstIterator(const HashMap& hashMap, const typename BucketConstIterator& bucketIterator, const typename ChainConstIterator& chainIterator) :
		mOwner(&hashMap), mBucketIterator(bucketIterator), mChainIterator(chainIterator)
	{
	}

	template<typename TKey, typename TData>
	inline typename const HashMap<TKey, TData>::PairType& HashMap<TKey, TData>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("ConstIterator invalid.");
		}

		return *mChainIterator;
	}

	template<typename TKey, typename TData>
	inline typename const HashMap<TKey, TData>::PairType* HashMap<TKey, TData>::ConstIterator::operator->() const
	{
		return &(this->operator*());
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ConstIterator::operator==(const ConstIterator& rhs) const noexcept
	{
		return !(operator!=(rhs));
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ConstIterator::operator!=(const ConstIterator& rhs) const noexcept
	{
		return (mOwner != rhs.mOwner || mChainIterator != rhs.mChainIterator);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator& HashMap<TKey, TData>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("ConstIterator invalid.");
		}

		if (mBucketIterator == mOwner->mBuckets.end())
		{
			throw std::out_of_range("ConstIterator out of bounds.");
		}

		if (mChainIterator != mBucketIterator->end())
		{
			++mChainIterator;
		}

		if (mChainIterator == mBucketIterator->end())
		{
			while (++mBucketIterator != mOwner->mBuckets.end() && mBucketIterator->IsEmpty());

			if (mBucketIterator != mOwner->mBuckets.end())
			{
				mChainIterator = mBucketIterator->begin();
			}
			else
			{
				mChainIterator = (mBucketIterator - 1)->end();
			}
		}

		return *this;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::ConstIterator::operator++(int)
	{
		ConstIterator it = ConstIterator(*this);
		++(*this);
		return it;
	}
#pragma endregion ConstIterator

#pragma region Constructors, Destructor, Assignment
	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(const size_t bucketCount, const KeyEqualityFunctor keyEqualityFunctor, const HashFunctor hashFunctor) :
		mBuckets(0, BucketType::EqualityFunctor()), mKeyEqualityFunctor(keyEqualityFunctor), mHashFunctor(hashFunctor)
	{		
		assert(bucketCount > 0);

		mBuckets.Resize(bucketCount, ChainType(ChainType::EqualityFunctor()));
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(HashMap&& rhs) noexcept :
		mBuckets(std::move(rhs.mBuckets)), mSize(rhs.mSize), mKeyEqualityFunctor(rhs.mKeyEqualityFunctor), mHashFunctor(rhs.mHashFunctor)
	{
		rhs.mSize = 0;
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>& HashMap<TKey, TData>::operator=(HashMap&& rhs) noexcept
	{
		mBuckets = std::move(rhs.mBuckets);
		mSize = rhs.mSize;
		mKeyEqualityFunctor = rhs.mKeyEqualityFunctor;
		mHashFunctor = rhs.mHashFunctor;

		rhs.mSize = 0;

		return *this;
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(const std::initializer_list<PairType> rhs, const size_t bucketCount, const KeyEqualityFunctor keyEqualityFunctor, const HashFunctor hashFunctor) :
		mBuckets(0, BucketType::EqualityFunctor()), mKeyEqualityFunctor(keyEqualityFunctor), mHashFunctor(hashFunctor)
	{
		assert(bucketCount > 0);

		mBuckets.Resize(bucketCount, ChainType(ChainType::EqualityFunctor()));

		for (auto pair : rhs)
		{
			Insert(pair);
		}
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>& HashMap<TKey, TData>::operator=(std::initializer_list<PairType> rhs)
	{
		Clear();

		for (auto pair : rhs)
		{
			Insert(pair);
		}

		return *this;
	}
#pragma endregion Constructors, Destructor, Assignment

#pragma region Size and Capacity
	template<typename TKey, typename TData>
	inline std::size_t HashMap<TKey, TData>::Size() const
	{
		return mSize;
	}

	template<typename TKey, typename TData>
	inline std::size_t HashMap<TKey, TData>::BucketCount() const
	{
		return mBuckets.Capacity();
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::IsEmpty() const
	{
		return mSize == 0;
	}

	template<typename TKey, typename TData>
	inline float HashMap<TKey, TData>::LoadFactor() const
	{
		return static_cast<float>(mSize) / mBuckets.Capacity();
	}

	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Rehash(const std::size_t bucketCount)
	{
		HashMap newHash = HashMap(bucketCount, mKeyEqualityFunctor, mHashFunctor);

		for (auto it = begin(); it != end(); ++it)
		{
			newHash.Insert(*it);
		}

		*this = std::move(newHash);
	}
#pragma endregion Size and Capacity

#pragma region Iterator Accessors
	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::begin()
	{
		if (mSize == 0)
		{
			return end();
		}

		BucketIterator bucketIterator = mBuckets.begin();
		ChainIterator chainIterator = bucketIterator->begin();

		while (bucketIterator != mBuckets.end() && bucketIterator->IsEmpty())
		{
			++bucketIterator;

			if (bucketIterator != mBuckets.end())
			{
				chainIterator = bucketIterator->begin();
			}
		}

		return Iterator(*this, bucketIterator, chainIterator);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::begin() const
	{
		return ConstIterator(const_cast<HashMap<TKey, TData>*>(this)->begin());
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cbegin() const
	{
		return ConstIterator(const_cast<HashMap<TKey, TData>*>(this)->begin());
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::end()
	{
		return Iterator(*this, mBuckets.end(), (mBuckets.end() - 1)->end());
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::end() const
	{
		return ConstIterator(*this, mBuckets.end(), (mBuckets.end() - 1)->end());
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cend() const
	{
		return ConstIterator(*this, mBuckets.end(), (mBuckets.end() - 1)->end());
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Find(const TKey& key)
	{
		std::size_t index;
		return Find(key, index);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::Find(const TKey& key) const
	{
		std::size_t index;
		return ConstIterator(const_cast<HashMap<TKey, TData>*>(this)->Find(key, index));
	}
#pragma endregion Iterator Accessors

#pragma region Element Accessors
	template<typename TKey, typename TData>
	inline TData& HashMap<TKey, TData>::At(const TKey& key)
	{
		Iterator it = Find(key);

		if (it == end())
		{
			throw std::out_of_range("TKey not found.");
		}

		return it->second;
	}

	template<typename TKey, typename TData>
	inline const TData& HashMap<TKey, TData>::At(const TKey& key) const
	{
		ConstIterator it = Find(key);

		if (it == end())
		{
			throw std::out_of_range("TKey not found.");
		}

		return it->second;
	}

	template<typename TKey, typename TData>
	inline TData& HashMap<TKey, TData>::operator[](const TKey& key)
	{
		return Insert({ key, TData() }).first->second;
	}

	template<typename TKey, typename TData>
	inline const TData& HashMap<TKey, TData>::operator[](const TKey& key) const
	{
		return At(key);
	}
	
	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ContainsKey(const TKey& key) const
	{
		return Find(key) != end();
	}
	
	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ContainsKey(const TKey& key, TData& dataOut)
	{
		auto it = Find(key);
			
		if (it != end())
		{
			dataOut = it->second;
			return true;
		}

		return false;
	}
#pragma endregion Element Accessors

#pragma region Modifiers
	template<typename TKey, typename TData>
	inline std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(const PairType& entry)
	{
		std::size_t index;
		auto it = Find(entry.first, index);
		
		bool alreadyExists = it != end();
		
		if (!alreadyExists)
		{
			it.mBucketIterator = mBuckets.begin()[index];
			it.mChainIterator = mBuckets[index].InsertAfter(mBuckets[index].end(), entry);
			++mSize;
		}

		return { it, !alreadyExists };
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Remove(const TKey& key)
	{
		std::size_t index;
		auto it = Find(key, index);
		return it.mBucketIterator->Remove(it.mChainIterator);
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Remove(const Iterator& it)
	{
		if (it.mOwner != this || it == end()) return false;

		return it.mBucketIterator->Remove(it.mChainIterator);
	}

	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Clear()
	{
		for (ChainType& chain : mBuckets)
		{
			chain.Clear();
		}

		mSize = 0;
	}
#pragma endregion Modifiers

#pragma region Helper Methods
	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Find(const TKey& key, std::size_t& indexOut)
	{
		indexOut = mHashFunctor(key) % mBuckets.Capacity();

		ChainType& chain = mBuckets[indexOut];

		ChainIterator chainIterator = chain.begin();
		for (; chainIterator != chain.end(); ++chainIterator)
		{
			if (mKeyEqualityFunctor(key, chainIterator->first))
			{
				break;
			}
		}

		if (chainIterator != chain.end())
		{
			return Iterator(*this, mBuckets.begin()[indexOut], chainIterator);
		}

		return end();
	}
#pragma endregion Helper Methods
}