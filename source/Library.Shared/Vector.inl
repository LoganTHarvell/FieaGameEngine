#include "Vector.h"	

namespace Library
{
#pragma region Default Functors
	template<typename T>
	inline constexpr std::size_t Vector<T>::DefaultReserveFunctor::operator()(const std::size_t, const std::size_t capacity) const
	{
		return static_cast<std::size_t>(capacity * 1.5);
	}
#pragma endregion Default Functors

#pragma region Iterator
	template <typename T>
	inline Vector<T>::Iterator::Iterator(Vector& owner, const std::size_t index) :
		mOwner(&owner), mIndex(index)
	{
	}

	template<typename T>
	inline T& Vector<T>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}

		return mOwner->At(mIndex);
	}

	template<typename T>
	inline T* Vector<T>::Iterator::operator->() const
	{
		return &(this->operator*());
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
			throw std::out_of_range("Iterator out of bounds.");
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
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator+=(const std::size_t rhs)
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}

		if (mIndex + rhs > mOwner->Size())
		{
			throw std::out_of_range("Iterator out of bounds.");
		}

		mIndex += rhs;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator+(const std::size_t rhs) const
	{
		return Iterator(*this) += rhs;
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}

		if (mIndex == 0)
		{
			throw std::out_of_range("Iterator out of bounds.");
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
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator-=(const std::size_t rhs)
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator invalid.");
		}

		if (mIndex < rhs)
		{
			throw std::out_of_range("Iterator out of bounds.");
		}

		mIndex -= rhs;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator-(const std::size_t rhs) const
	{
		return Iterator(*this) -= rhs;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator[](const std::size_t rhs) const
	{
		return Iterator(*this) + rhs;
	}
#pragma endregion Iterator

#pragma region ConstIterator
	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Iterator& it) :
		mOwner(it.mOwner), mIndex(it.mIndex)
	{
	}

	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Vector& owner, const std::size_t index) :
		mOwner(&owner), mIndex(index)
	{
	}

	template<typename T>
	inline const T& Vector<T>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("ConstIterator invalid.");
		}

		return mOwner->At(mIndex);
	}

	template<typename T>
	inline const T* Vector<T>::ConstIterator::operator->() const
	{
		return &(this->operator*());
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
			throw std::runtime_error("ConstIterator invalid.");
		}

		if (mIndex >= mOwner->Size())
		{
			throw std::out_of_range("ConstIterator out of bounds.");
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
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator+=(const std::size_t rhs)
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("ConstIterator invalid.");
		}

		if (mIndex + rhs > mOwner->Size())
		{
			throw std::out_of_range("ConstIterator out of bounds.");
		}

		mIndex += rhs;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator+(const std::size_t rhs) const
	{
		return ConstIterator(*this) += rhs;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("ConstIterator invalid.");
		}

		if (mIndex == 0)
		{
			throw std::out_of_range("ConstIterator out of bounds.");
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
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator-=(const std::size_t rhs)
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("ConstIterator invalid.");
		}

		if (mIndex < rhs)
		{
			throw std::out_of_range("ConstIterator out of bounds.");
		}

		mIndex -= rhs;

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator-(const std::size_t rhs) const
	{
		return ConstIterator(*this) -= rhs;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator[](const std::size_t rhs) const
	{
		return ConstIterator(*this) + rhs;
	}
#pragma endregion ConstIterator

#pragma region Constructors, Destructor, Assignment
	template <typename T>
	inline Vector<T>::Vector(const std::size_t capacity, const EqualityFunctor equalityFunctor, const ReserveFunctor reserveFunctor) :
		mEqualityFunctor(equalityFunctor), mReserveFunctor(reserveFunctor)
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

		if (mIntData != nullptr)
		{
			free(mIntData);
			mIntData = nullptr;
		}

		mCapacity = 0;
	}

	template<typename T>
	inline Vector<T>::Vector(const Vector& rhs) :
		mEqualityFunctor(rhs.mEqualityFunctor), mReserveFunctor(rhs.mReserveFunctor)
	{
		Reserve(rhs.mCapacity);

		for (std::size_t i = 0; i < rhs.mSize; ++i)
		{
			PushBack(rhs[i]);
		}
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			ShrinkToFit();
			Reserve(rhs.mCapacity);

			for (std::size_t i = 0; i < rhs.mSize; ++i)
			{
				PushBack(rhs[i]);
			}

			mReserveFunctor = rhs.mReserveFunctor;
			mEqualityFunctor = rhs.mEqualityFunctor;
		}

		return *this;
	}

	template<typename T>
	inline Vector<T>::Vector(Vector&& rhs) noexcept :
		mIntData(rhs.mIntData), mSize(rhs.mSize), mCapacity(rhs.mCapacity), mEqualityFunctor(rhs.mEqualityFunctor), mReserveFunctor(rhs.mReserveFunctor)
	{
		rhs.mIntData = nullptr;
		rhs.mSize = 0;
		rhs.mCapacity = 0;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept
	{
		if (this != &rhs)
		{
			if (mCapacity > 0)
			{
				Clear();
				free(mIntData);
			}

			mIntData = rhs.mIntData;
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mReserveFunctor = rhs.mReserveFunctor;
			mEqualityFunctor = rhs.mEqualityFunctor;

			rhs.mIntData = nullptr;
			rhs.mSize = 0;
			rhs.mCapacity = 0;
		}

		return *this;
	}

	template<typename T>
	inline Vector<T>::Vector(const std::initializer_list<T> rhs, const EqualityFunctor equalityFunctor, const ReserveFunctor reserveFunctor) :
		mEqualityFunctor(equalityFunctor), mReserveFunctor(reserveFunctor)
	{
		Reserve(rhs.size());

		for (const auto& value : rhs)
		{
			PushBack(value);
		}
	}
	template<typename T>
	inline Vector<T>& Vector<T>::operator=(const std::initializer_list<T> rhs)
	{
		Clear();
		ShrinkToFit();
		Reserve(rhs.size());

		for (const auto& value : rhs)
		{
			PushBack(value);
		}

		return *this;
	}
#pragma endregion Constructors, Destructor, Assignment

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

#pragma region Size and Capacity
	template<typename T>
	inline std::size_t Vector<T>::Size() const
	{
		return mSize;
	}

	template<typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template<typename T>
	inline std::size_t Vector<T>::Capacity() const
	{
		return mCapacity;
	}

	template<typename T>
	inline void Vector<T>::Reserve(const std::size_t capacity)
	{
		if (capacity > mCapacity)
		{
			void* newMemory = realloc(mIntData, capacity * sizeof(T));

			assert(newMemory != nullptr);
			mIntData = reinterpret_cast<T*>(newMemory);
			mCapacity = capacity;
		}
	}

	template<typename T>
	inline void Vector<T>::Resize(const std::size_t size)
	{
		if (size > mSize)
		{
			Reserve(size);

			for (std::size_t i = mSize; i < size; ++i)
			{
				new(mIntData + i)T();
			}
		}
		else if (size < mSize)
		{
			for (std::size_t i = size; i < mSize; ++i)
			{
				mIntData[i].~T();
			}
		}

		mSize = size;
	}

	template<typename T>
	inline void Vector<T>::Resize(const std::size_t size, const T& value)
	{
		if (size > mSize)
		{
			Reserve(size);

			for (std::size_t i = mSize; i < size; ++i)
			{
				new(mIntData + i)T(value);
			}
		}
		else if (size < mSize)
		{
			for (std::size_t i = size; i < mSize; ++i)
			{
				mIntData[i].~T();
			}
		}

		mSize = size;
	}

	template<typename T>
	inline void Vector<T>::ShrinkToFit()
	{
		if (mSize == 0)
		{
			free(mIntData);
			mIntData = nullptr;
		}
		else if (mSize < mCapacity)
		{
			void* newMemory = realloc(mIntData, mSize * sizeof(T));

			assert(newMemory != nullptr);
			mIntData = reinterpret_cast<T*>(newMemory);
		}

		mCapacity = mSize;
	}
#pragma endregion Size and Capacity

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
	inline typename Vector<T>::Iterator Vector<T>::Find(const T& value)
	{
		if (!mEqualityFunctor) throw std::runtime_error("Missing equality functor.");

		for (std::size_t i = 0; i < mSize; ++i)
		{
			if (mEqualityFunctor(mIntData[i], value))
			{
				return Iterator(*this, i);
			}
		}

		return Iterator(*this, mSize);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::Find(const T& value) const
	{
		Iterator it = const_cast<Vector<T>*>(this)->Find(value);
		return ConstIterator(it);
	}
#pragma endregion Iterator Accessors

#pragma region Element Accessors
	template<typename T>
	inline T& Vector<T>::Front()
	{
		if (mSize == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return mIntData[0];
	}

	template<typename T>
	inline const T& Vector<T>::Front() const
	{
		if (mSize == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return mIntData[0];
	}

	template<typename T>
	inline T& Vector<T>::Back()
	{
		if (mSize == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return mIntData[mSize - 1];
	}

	template<typename T>
	inline const T& Vector<T>::Back() const
	{
		if (mSize == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return mIntData[mSize - 1];
	}

	template<typename T>
	inline T& Vector<T>::At(const std::size_t index)
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Index is out of bounds.");
		}

		return mIntData[index];
	}

	template<typename T>
	inline const T& Vector<T>::At(const std::size_t index) const
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Index is out of bounds.");
		}

		return mIntData[index];
	}

	template<typename T>
	inline T& Vector<T>::operator[](const std::size_t index)
	{
		return At(index);
	}

	template<typename T>
	inline const T& Vector<T>::operator[](const std::size_t index) const
	{
		return At(index);
	}
#pragma endregion Element Accessors

#pragma region Modifiers
	template<typename T>
	inline void Vector<T>::PushBack(const T& data)
	{
		if (!mReserveFunctor) throw std::runtime_error("Missing reserve functor.");

		if (mCapacity <= mSize)
		{
			std::size_t newCapacity = mReserveFunctor(mCapacity, mSize);
			Reserve(std::max(newCapacity, mCapacity + 1));
		}

		new(mIntData + mSize++)T(data);
	}

	template<typename T>
	inline void Vector<T>::PopBack()
	{
		if (mSize > 0)
		{
			mIntData[--mSize].~T();
		}
	}

	template<typename T>
	inline bool Vector<T>::Remove(const T& value)
	{
		if (!mEqualityFunctor) throw std::runtime_error("Missing equality functor.");

		for (std::size_t i = 0; i < mSize; ++i)
		{
			if (mEqualityFunctor(mIntData[i], value))
			{
				mIntData[i].~T();
				memmove(&mIntData[i], &mIntData[i + 1], sizeof(T) * (mSize - i));

				--mSize;
				return true;
			}
		}

		return false;
	}

	template<typename T>
	inline bool Vector<T>::Remove(const Iterator& it)
	{
		if (it.mOwner != this) throw std::runtime_error("Invalid iterator.");
		
		bool isRemoved = false;

		if (it != end())
		{
			if (it.mIndex == mSize - 1)
			{
				PopBack();
			}
			else
			{
				mIntData[it.mIndex].~T();
				memmove(&mIntData[it.mIndex], &mIntData[it.mIndex + 1], sizeof(T) * (mSize - it.mIndex));
				--mSize;
			}

			isRemoved = true;
		}

		return isRemoved;
	}

	template<typename T>
	inline void Vector<T>::Clear()
	{
		for (std::size_t i = 0; i < mSize; ++i)
		{
			mIntData[i].~T();
		}

		mSize = 0;
	}
#pragma endregion Modifiers
}