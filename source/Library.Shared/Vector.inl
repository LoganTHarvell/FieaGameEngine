#include "Vector.h"	

namespace Library 
{
#pragma region Iterator
	template <typename T>
	Vector<T>::Iterator::Iterator(Vector<T>& owner, const size_t index) :
		mOwner(&owner), mIndex(index)
	{
	}

	template<typename T>
	inline T& Vector<T>::Iterator::operator*() const
	{
		return mOwner->operator[](mIndex);
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator==(const Iterator& rhs) const noexcept
	{
		return !(operator!=(rhs));
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator!=(const Iterator& rhs) const noexcept
	{
		return (mOwner != rhs.mOwner || mIndex != rhs.mIndex);
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}
		
		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Iterator out of bounds.");
		}

		++mIndex;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator it = Iterator(*this);
		++(*this);
		return it;
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator+=(const size_t rhs)
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}

		if (mIndex + rhs > mOwner->Size())
		{
			throw std::runtime_error("Iterator out of bounds.");
		}

		mIndex += rhs;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator+(const size_t rhs)
	{
		Iterator it = Iterator(*this);
		it += rhs;
		return it;
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}
		else if (mIndex == 0)
		{
			throw std::runtime_error("Iterator out of bounds.");
		}

		--mIndex;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
	{
		Iterator it = Iterator(*this);
		--(*this);
		return it;
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator-=(const size_t rhs)
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}

		if (mIndex < rhs)
		{
			throw std::runtime_error("Iterator out of bounds.");
		}

		mIndex -= rhs;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator-(const size_t rhs)
	{
		Iterator it = Iterator(*this);
		it -= rhs;
		return it;
	}
#pragma endregion Iterator

#pragma region ConstIterator
	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Iterator& rhs) :
		mOwner(rhs.mOwner), mIndex(rhs.mIndex)
	{
	}

	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Vector& owner, const size_t index) :
		mOwner(&owner), mIndex(index)
	{
	}

	template<typename T>
	inline const T& Vector<T>::ConstIterator::operator*() const
	{
		return mOwner->operator[](mIndex);
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator==(const ConstIterator& rhs) const noexcept
	{
		return !operator!=(rhs);
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator!=(const ConstIterator& rhs) const noexcept
	{
		return (mOwner != rhs.mOwner || mIndex != rhs.mIndex);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}
		
		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Iterator attempted to go past end.");
		}

		++mIndex;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator it = ConstIterator(*this);
		++(*this);
		return it;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator+=(const size_t rhs)
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}

		if (mIndex + rhs > mOwner->Size())
		{
			throw std::runtime_error("Iterator out of bounds.");
		}

		mIndex += rhs;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator+(const size_t rhs)
	{
		ConstIterator it = ConstIterator(*this);
		it += rhs;
		return it;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}
		
		if (mIndex == 0)
		{
			throw std::runtime_error("Iterator out of bounds.");
		}

		--mIndex;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator--(int)
	{
		ConstIterator it = ConstIterator(*this);
		--(*this);
		return it;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator-=(const size_t rhs)
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}

		if (mIndex < rhs)
		{
			throw std::runtime_error("Iterator out of bounds.");
		}

		mIndex -= rhs;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator-(const size_t rhs)
	{
		ConstIterator it = ConstIterator(*this);
		it -= rhs;
		return it;
	}
#pragma endregion ConstIterator

#pragma region Constructors and Destructor
	template <typename T>
	Vector<T>::Vector(const size_t capacity)
	{
		if (capacity > 0)
		{
			Reserve(capacity);
		}
	}

	template<typename T>
	inline Vector<T>::~Vector()
	{
		Clear();

		if (mData != nullptr)
		{
			free(mData);
			mData = nullptr;
		}

		mCapacity = 0;
	}

	template<typename T>
	inline Vector<T>::Vector(const Vector& rhs)
	{
		Reserve(rhs.Capacity());

		for (size_t i = 0; i < rhs.mSize; ++i)
		{
			PushBack(rhs[i]);
		}
	}

	template<typename T>
	inline Vector<T>::Vector(Vector&& rhs) noexcept :
		mData(rhs.mData), mSize(rhs.mSize), mCapacity(rhs.mCapacity)
	{
		rhs.mData = nullptr;
		rhs.mSize = 0;
		rhs.mCapacity = 0;
	}

	template<typename T>
	inline Vector<T>::Vector(const std::initializer_list<T> rhs)
	{
		Reserve(rhs.size());

		for (const auto& value : rhs)
		{
			PushBack(value);
		}
	}
#pragma endregion Constructors and Destructor

#pragma region Assignment Operators
	template<typename T>
	inline Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			Reserve(rhs.Capacity());

			for (size_t i = 0; i < rhs.mSize; ++i)
			{
				PushBack(rhs[i]);
			}
		}

		return *this;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();

			if (mData != nullptr)
			{
				free(mData);
			}

			mData = rhs.mData;
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;

			rhs.mData = nullptr;
			rhs.mSize = 0;
			rhs.mCapacity = 0;
		}

		return *this;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(const std::initializer_list<T> rhs)
	{
		Clear();

		Reserve(rhs.size());

		for (const auto& value : rhs)
		{
			PushBack(value);
		}

		return *this;
	}
#pragma endregion Assignment Operators

#pragma region Boolean Operators
	template<typename T>
	inline bool Vector<T>::operator==(const Vector& rhs) const noexcept
	{
		if (this == &rhs)
		{
			return true;
		}

		if (mSize != rhs.mSize)
		{
			return false;
		}

		return std::equal(begin(), end(), rhs.begin());
	}

	template<typename T>
	inline bool Vector<T>::operator!=(const Vector& rhs) const noexcept
	{
		return !(operator==(rhs));
	}
#pragma endregion Boolean Operators

#pragma region Iterator Accessors
	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator(*this, 0);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return ConstIterator(*this, 0);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator(*this, 0);
	}
	
	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator(*this, mSize);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return ConstIterator(*this, mSize);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator(*this, mSize);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Find(const T& value, const EqualityFunctor equal)
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			if (equal(mData[i], value))
			{
				return Iterator(*this, i);
			}
		}

		return Iterator(*this, mSize);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::Find(const T& value, const EqualityFunctor equal) const
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			if (equal(mData[i], value))
			{
				return ConstIterator(*this, i);
			}
		}

		return ConstIterator(*this, mSize);
	}
#pragma endregion Iterator Accessors

#pragma region Size and Capacity
	template<typename T>
	inline size_t Vector<T>::Size() const
	{
		return mSize;
	}

	template<typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template<typename T>
	inline size_t Vector<T>::Capacity() const
	{
		return mCapacity;
	}

	template<typename T>
	inline void Vector<T>::Reserve(const size_t capacity)
	{
		if (capacity > mCapacity)
		{
			void* newMemory = realloc(mData, capacity * sizeof(T));
			
			if (newMemory == nullptr)
			{
				throw std::runtime_error("Insufficient memory.");
			}
	
			mData = reinterpret_cast<T*>(newMemory);
			mCapacity = capacity;
		}
	}

	template<typename T>
	inline void Vector<T>::Resize(const size_t size)
	{
		if (size > mSize)
		{
			Reserve(size);

			for (size_t i = mSize; i < size; ++i)
			{
				new(mData + i)T();
			}
		}
		else if (size < mSize) 
		{
			for (size_t i = size; i < mSize; ++i)
			{
				mData[i].~T();
			}
		}

		mSize = size;
	}

	template<typename T>
	inline void Vector<T>::Resize(const size_t size, const T& value)
	{
		if (size > mSize)
		{
			Reserve(size);

			for (size_t i = mSize; i < size; ++i)
			{
				new(mData + i)T(value);
			}
		}
		else if (size < mSize) 
		{
			for (size_t i = size; i < mSize; ++i)
			{
				mData[i].~T();
			}
		}

		mSize = size;
	}
	
	template<typename T>
	inline void Vector<T>::ShrinkToFit()
	{
		if (mSize < mCapacity)
		{
			void* newMemory = realloc(mData, capacity * sizeof(T));

			if (newMemory == nullptr)
			{
				throw std::runtime_error("Unable to reallocate memory.");
			}

			mData = reinterpret_cast<T*>(newMemory);
			mCapacity = mSize;
		}
	}
#pragma endregion Size and Capacity

#pragma region Element Accessors
	template<typename T>
	inline T& Vector<T>::At(const size_t index)
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Index is out of bounds.");
		}

		return mData[index];
	}

	template<typename T>
	inline const T& Vector<T>::At(const size_t index) const
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Index is out of bounds.");
		}

		return mData[index];
	}

	template<typename T>
	inline T& Vector<T>::operator[](const size_t index)
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Index is out of bounds.");
		}

		return mData[index];
	}

	template<typename T>
	inline const T& Vector<T>::operator[](const size_t index) const
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Index is out of bounds.");
		}

		return mData[index];
	}

	template<typename T>
	inline T& Vector<T>::Front()
	{
		if (mSize == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return mData[0];
	}

	template<typename T>
	inline const T& Vector<T>::Front() const
	{
		if (mSize == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return mData[0];
	}

	template<typename T>
	inline T& Vector<T>::Back()
	{
		if (mSize == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return mData[mSize - 1];
	}

	template<typename T>
	inline const T& Vector<T>::Back() const
	{
		if (mSize == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return mData[mSize - 1];
	}
#pragma endregion Element Accessors

#pragma region Modifiers
	template<typename T>
	inline void Vector<T>::PushBack(const T& data, const ReserveStrategy reserveStrategy)
	{
		if (mCapacity <= mSize)
		{
			size_t newCapacity = reserveStrategy(mCapacity, mSize);
			Reserve(newCapacity > mCapacity ? newCapacity : mCapacity + 1);
		}

		new(mData + mSize++)T(data);
	}

	template<typename T>
	inline void Vector<T>::PopBack()
	{
		if (mSize > 0)
		{
			mData[--mSize].~T();
		}
	}

	template<typename T>
	inline bool Vector<T>::Remove(const T& value, const EqualityFunctor equal)
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			if (equal(mData[i], value))
			{
				mData[i].~T();
				memmove(&mData[i], &mData[i + 1], sizeof(T) * (mSize - i));

				--mSize;
				return true;
			}
		}

		return false;
	}

	template<typename T>
	inline bool Vector<T>::Remove(const Iterator& it)
	{
		if (it.mOwner != this)
		{
			throw std::runtime_error("Invalid iterator.");
		}

		bool isRemoved = false;

		if (it != end())
		{
			if (it.mIndex == mSize - 1)
			{
				PopBack();
			}
			else
			{
				mData[it.mIndex].~T();
				memmove(&mData[it.mIndex], &mData[it.mIndex + 1], sizeof(T) * (mSize - it.mIndex));
				--mSize;
			}

			isRemoved = true;
		}

		return isRemoved;
	}

	template<typename T>
	inline void Vector<T>::Clear()
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			mData[i].~T();
		}

		mSize = 0;
	}
#pragma endregion Modifiers
}