#include "HashMap.h"
#pragma once

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
		if (mOwner != rhs.mOwner) return true;
		// TODO: Set Equality functor for SLists
		return mChainIterator == rhs.mChainIterator;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator& HashMap<TKey, TData>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}

		if (mBucketIterator == mBuckets.end())
		{
			throw std::out_of_range("Iterator out of bounds.");
		}

		if (mChainIterator != mBucketIterator->end())
		{
			++mChainIterator;
		}

		if (mChainIterator == mBucketIterator->end())
		{
			while (++mBucketIterator != mBuckets.end() && mBucketIterator->IsEmpty());

			if (mBucketIterator != mBuckets.end())
			{
				mChainIterator = mBucketIterator->begin();
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
	inline HashMap<TKey, TData>::ConstIterator::ConstIterator(const HashMap& hashMap, const typename BucketType::Iterator& bucketIterator, const typename ChainType::Iterator& chainIterator) :
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
		if (mOwner != rhs.mOwner) return;

		bool isEqual = mKeyEqualityFunctor(mChainIterator->first, rhs.mChainIterator->first);

		if (isEqual)
		{
			isEqual = mDataEqualityFunctor(mChainIterator->second, rhs.mChainIterator->second);
		}

		return isEqual;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator& HashMap<TKey, TData>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("ConstIterator invalid.");
		}

		if (mBucketIterator == mBuckets.end())
		{
			throw std::out_of_range("ConstIterator out of bounds.");
		}

		if (mChainIterator != mBucketIterator->end())
		{
			++mChainIterator;
		}

		if (mChainIterator == mBucketIterator->end())
		{
			while (++mBucketIterator != mBuckets.end() && mBucketIterator->IsEmpty());

			if (mBucketIterator != mBuckets.end())
			{
				mChainIterator = mBucketIterator->begin();
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
	inline HashMap<TKey, TData>::HashMap(const size_t capacity, const KeyEqualityFunctor keyEqualityFunctor, const DataEqualityFunctor dataEqualityFunctor, const HashFunctor hashFunctor) :
		mKeyEqualityFunctor(keyEqualityFunctor), mDataEqualityFunctor(dataEqualityFunctor), mHashFunctor(hashFunctor)
	{
		mBuckets.Resize(capacity);	
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(const HashMap& rhs) :
		mBuckets(rhs.mBuckets), mSize(rhs.mSize), mKeyEqualityFunctor(rhs.mKeyEqualityFunctor), mDataEqualityFunctor(mDataEqualityFunctor), mHashFunctor(rhs.mHashFunctor)
	{
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(HashMap&& rhs) noexcept :
		mBuckets(std::move(rhs.mBuckets)), mSize(rhs.mSize), mKeyEqualityFunctor(rhs.mKeyEqualityFunctor), mDataEqualityFunctor(mDataEqualityFunctor), mHashFunctor(rhs.mHashFunctor)
	{
	}
#pragma endregion Constructors

#pragma region Assignment Operators
	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>& HashMap<TKey, TData>::operator=(const HashMap& rhs)
	{
		mBuckets = rhs.mBuckets;
		mSize = rhs.mSize;
		mKeyEqualityFunctor = rhs.mKeyEqualityFunctor;
		mDataEqualityFunctor = rhs.mDataEqualityFunctor;
		mHashFunctor = rhs.mHashFunctor;
	}
	
	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>& HashMap<TKey, TData>::operator=(HashMap&& rhs) noexcept
	{
		mBuckets = std::move(rhs.mBuckets);
		mSize = rhs.mSize;
		mKeyEqualityFunctor = rhs.mKeyEqualityFunctor;
		mDataEqualityFunctor = rhs.mDataEqualityFunctor;
		mHashFunctor = rhs.mHashFunctor;

		rhs.mSize = 0;
	}
#pragma endregion Assignment Operators

#pragma region Size and Capacity
	template<typename TKey, typename TData>
	inline std::size_t HashMap<TKey, TData>::Size() const
	{
		return mSize;
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
			return Iterator(*this, mBuckets.end(), (mBuckets.end() - 1)->end());
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
		Iterator it = const_cast<HashMap<TKey, TData>>(*this).begin();
		return ConstIterator(it);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cbegin() const
	{
		Iterator it = const_cast<HashMap<TKey, TData>>(*this).begin();
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
		return Iterator(*this, mBuckets.end(), (mBuckets.end() - 1)->end());
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cend() const
	{
		return Iterator(*this, mBuckets.end(), (mBuckets.end() - 1)->end());
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Find(const TKey& value)
	{
		if (!mKeyEqualityFunctor || !mDataEqualityFunctor)
		{
			throw std::runtime_error("Missing equality functor.");
		}

		for (std::size_t i = 0; i < mSize; ++i)
		{
			if (mEqualityFunctor(mData[i], value))
			{
				return Iterator(*this, i);
			}
		}

		return Iterator(*this, mSize);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::Find(const TKey& value) const
	{
		if (!mKeyEqualityFunctor || !mDataEqualityFunctor)
		{
			throw std::runtime_error("Missing equality functor.");
		}

		for (std::size_t i = 0; i < mSize; ++i)
		{
			if (mEqualityFunctor(mData[i], value))
			{
				return ConstIterator(*this, i);
			}
		}

		return ConstIterator(*this, mSize);
	}
#pragma endregion Iterator Accessors
}