#pragma once

#include "HashMap.h"

namespace Library
{
#pragma region Iterator
	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::Iterator::Iterator(HashMap& hashMap, const typename BucketType::Iterator& bucketIterator, const typename ChainType::Iterator& chainIterator) :
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
	inline HashMap<TKey, TData>::ConstIterator::ConstIterator(const HashMap& hashMap, const typename BucketType::ConstIterator& bucketIterator, const typename ChainType::ConstIterator& chainIterator) :
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

#pragma region Constructors
	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(const size_t bucketCount, const KeyEqualityFunctor keyEqualityFunctor, const HashFunctor hashFunctor) :
		mKeyEqualityFunctor(keyEqualityFunctor), mHashFunctor(hashFunctor)
	{		
		mBuckets.Resize(std::max(bucketCount, std::size_t(1)));
	}
#pragma endregion Constructors

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
#pragma endregion Size and Capacity

#pragma region Iterator Accessors
	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::begin()
	{
		if (mSize == 0)
		{
			return end();
		}

		BucketType::Iterator bucketIterator = mBuckets.begin();
		ChainType::Iterator chainIterator = bucketIterator->begin();

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
		Iterator it = const_cast<HashMap<TKey, TData>*>(this)->begin();
		return ConstIterator(it);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cbegin() const
	{
		Iterator it = const_cast<HashMap<TKey, TData>*>(this)->begin();
		return ConstIterator(it);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::end()
	{
		return Iterator(*this, mBuckets.end(), (mBuckets.end() - 1)->end());
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::end() const
	{
		Iterator it = const_cast<HashMap<TKey, TData>*>(this)->end();
		return ConstIterator(it);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cend() const
	{
		Iterator it = const_cast<HashMap<TKey, TData>*>(this)->end();
		return ConstIterator(it);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Find(const TKey& key)
	{
		const std::size_t index = mHashFunctor(key) % mBuckets.Capacity();

		ChainType chain = mBuckets[index];

		ChainType::Iterator chainIterator = chain.begin();
		for (; chainIterator != chain.end(); ++chainIterator)
		{
			if (mKeyEqualityFunctor(key, chainIterator->first))
			{
				break;
			}
		}

		if (chainIterator != chain.end())
		{
			return Iterator(*this, mBuckets.begin()[index], chainIterator);
		}

		return end();
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::Find(const TKey& key) const
	{
		const std::size_t index = mHashFunctor(key) % mBuckets.Capacity();

		ChainType chain = mBuckets[index];

		ChainType::ConstIterator chainIterator = chain.cbegin();
		for (; chainIterator != chain.cend(); ++chainIterator)
		{
			if (mKeyEqualityFunctor(key, chainIterator->first))
			{
				break;
			}
		}

		if (chainIterator != chain.cend())
		{
			return ConstIterator(*this, mBuckets.cbegin()[index], chainIterator);
		}

		return cend();
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
#pragma endregion Element Accessors

#pragma region Modifiers
	template<typename TKey, typename TData>
	inline std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(const PairType& entry)
	{
		const std::size_t index = mHashFunctor(entry.first) % mBuckets.Capacity();
		
		ChainType& chain = mBuckets[index];

		ChainType::Iterator chainIterator = chain.begin();
		for (; chainIterator != chain.end(); ++chainIterator)
		{
			if (mKeyEqualityFunctor(entry.first, chainIterator->first))
			{
				break;
			}
		}
		
		bool alreadyExists = chainIterator != chain.end();
		
		if (!alreadyExists)
		{
			chainIterator = chain.InsertAfter(chainIterator, entry);
			++mSize;
		}

		return { Iterator(*this, mBuckets.begin()[index], chainIterator), !alreadyExists };
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Remove(const TKey& key)
	{
		const std::size_t index = mHashFunctor(key) % mBuckets.Capacity();

		ChainType chain = mBuckets[index];

		ChainType::Iterator chainIterator = chain.begin();
		for (; chainIterator != chain.end; ++chainIterator)
		{
			if (mKeyEqualityFunctor(key, chainIterator->first))
			{
				break;
			}
		}

		return chain.Remove(chainIterator);
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Remove(const Iterator& it)
	{
		const std::size_t index = mHashFunctor(it->first) % mBuckets.Capacity();

		ChainType chain = mBuckets[index];

		return chain.Remove(chainIterator);
	}

	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Clear()
	{
		for (ChainType chain : mBuckets)
		{
			chain.Clear();
		}

		mSize = 0;
	}
#pragma endregion Modifiers
}