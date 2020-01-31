#include "SList.h"

namespace Library
{
#pragma region Node
	template<typename T>
	inline SList<T>::Node::Node(const T& data, const std::shared_ptr<Node> next) :
		Data(data), Next(next)
	{
	}
#pragma endregion Node

#pragma region Iterator
	template<typename T>
	inline SList<T>::Iterator::Iterator(const SList& owner, const std::shared_ptr<Node> node) :
		mOwner(&owner), mNode(node)
	{
	}

	template<typename T>
	inline T& SList<T>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid Iterator.");
		}

		if (mNode == nullptr)
		{
			throw std::out_of_range("Iterator out of bounds.");
		}

		return mNode->Data;
	}

	template<typename T>
	inline T* SList<T>::Iterator::operator->() const
	{
		return &(this->operator*());
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator==(const Iterator& rhs) const noexcept
	{
		return !(operator!=(rhs));
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& rhs) const noexcept
	{
		return (mOwner != rhs.mOwner || mNode != rhs.mNode);
	}

	template<typename T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid Iterator.");
		}

		if (mNode == nullptr)
		{
			throw std::out_of_range("Iterator out of bounds.");
		}

		mNode = mNode->Next;

		return *this;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator it = Iterator(*this);
		++(*this);
		return it;
	}
#pragma endregion Iterator

#pragma region ConstIterator
	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const Iterator& it) :
		mOwner(it.mOwner), mNode(it.mNode)
	{
	}

	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const SList& owner, const std::shared_ptr<Node> node) :
		mOwner(&owner), mNode(node)
	{
	}

	template<typename T>
	inline const T& SList<T>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid ConstIterator.");
		}

		if (mNode == nullptr)
		{
			throw std::out_of_range("ConstIterator out of bounds.");
		}

		return mNode->Data;
	}

	template<typename T>
	inline const T* SList<T>::ConstIterator::operator->() const
	{
		return &(this->operator*());
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator==(const ConstIterator& rhs) const noexcept
	{
		return !operator!=(rhs);
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator!=(const ConstIterator& rhs) const noexcept
	{
		return (mOwner != rhs.mOwner || mNode != rhs.mNode);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid ConstIterator.");
		}

		if (mNode == nullptr)
		{
			throw std::out_of_range("ConstIterator out of bounds.");
		}

		mNode = mNode->Next;

		return *this;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator it = ConstIterator(*this);
		++(*this);
		return it;
	}
#pragma endregion ConstIterator

#pragma region Constructors, Destructor, Assignment
	template<typename T>
	inline SList<T>::SList(const EqualityFunctor equalityFunctor) :
		mEqualityFunctor(equalityFunctor)
	{
	}

	template<typename T>
	inline SList<T>::~SList()
	{
		Clear();
	}

	template<typename T>
	inline SList<T>::SList(const SList& rhs) :
		mEqualityFunctor(rhs.mEqualityFunctor)
	{
		for (const auto& value : rhs)
		{
			PushBack(value);
		}
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(const SList& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			for (const auto& value : rhs)
			{
				PushBack(value);
			}
		}

		mEqualityFunctor = rhs.mEqualityFunctor;

		return *this;
	}

	template<typename T>
	inline SList<T>::SList(SList&& rhs) noexcept :
		mSize(rhs.mSize), mFront(rhs.mFront), mBack(rhs.mBack), mEqualityFunctor(rhs.mEqualityFunctor)
	{
		rhs.mSize = 0;
		rhs.mFront = nullptr;
		rhs.mBack = nullptr;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(SList&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();

			mSize = rhs.mSize;
			mFront = rhs.mFront;
			mBack = rhs.mBack;
			mEqualityFunctor = rhs.mEqualityFunctor;

			rhs.mSize = 0;
			rhs.mFront = nullptr;
			rhs.mBack = nullptr;
		}

		return *this;
	}

	template<typename T>
	inline SList<T>::SList(const std::initializer_list<T> rhs, const EqualityFunctor equalityFunctor) :
		mEqualityFunctor(equalityFunctor)
	{
		for (const auto& value : rhs)
		{
			PushBack(value);
		}
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(const std::initializer_list<T> rhs)
	{
		Clear();

		for (const auto& value : rhs)
		{
			PushBack(value);
		}

		return *this;
	}
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
	template<typename T>
	inline bool SList<T>::operator==(const SList& rhs) const noexcept
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
	inline bool SList<T>::operator!=(const SList& rhs) const noexcept
	{
		return !(operator==(rhs));
	}
#pragma endregion Boolean Operators

#pragma region Size
	template<typename T>
	inline std::size_t SList<T>::Size() const
	{
		return mSize;
	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return mSize == 0;
	}
#pragma endregion Size

#pragma region Iterator Accessors
	template<typename T>
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator(*this, mFront);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return ConstIterator(*this, mFront);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator(*this, mFront);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator(*this, nullptr);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const
	{
		return ConstIterator(*this, nullptr);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator(*this, nullptr);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::Find(const T& value)
	{
		auto it = begin();
		for (; it != end(); ++it)
		{
			if (mEqualityFunctor(*it, value))
			{
				break;
			}
		}

		return it;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::Find(const T& value) const
	{
		auto it = cbegin();
		for (; it != cend(); ++it)
		{
			if (mEqualityFunctor(*it, value))
			{
				break;
			}
		}

		return it;
	}
#pragma endregion Iterator Accessors

#pragma region Element Accessors
	template<typename T>
	inline T& SList<T>::Front()
	{
		if (mSize == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return mFront->Data;
	}

	template<typename T>
	inline const T& SList<T>::Front() const
	{
		if (mSize == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return mFront->Data;
	}

	template<typename T>
	inline T& SList<T>::Back()
	{
		if (mSize == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return mBack->Data;
	}
	
	template<typename T>
	inline const T& SList<T>::Back() const
	{
		if (mSize == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return mBack->Data;
	}
#pragma endregion Element Accessors

#pragma region Modifiers
	template<typename T>
	inline void SList<T>::PushFront(const T& data)
	{
		mFront = std::make_shared<Node>(data, mFront);

		if (mSize == 0)
		{
			mBack = mFront;
		}

		mSize++;
	}

	template<typename T>
	inline void SList<T>::PushBack(const T& data)
	{
		std::shared_ptr<Node> newNode = std::make_shared<Node>(data);

		if (mSize == 0)
		{
			mBack = newNode;
			mFront = mBack;
		}
		else
		{
			mBack->Next = newNode;
			mBack = mBack->Next;
		}

		mSize++;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::InsertAfter(const Iterator& it, const T& data)
	{
		if (this != it.mOwner)
		{
			throw std::runtime_error("Invalid iterator.");
		}

		if (it == end())
		{
			PushBack(data);
			return Iterator(*this, mBack);
		}

		std::shared_ptr<Node> newNode = std::make_shared<Node>(data);
		newNode->Next = it.mNode->Next;
		it.mNode->Next = newNode;

		return Iterator(*this, newNode);
	}

	template<typename T>
	inline void SList<T>::PopFront()
	{
		if (mSize > 0)
		{
			mFront = mFront->Next;
			mSize--;

			if (mSize <= 1)
			{
				mBack = mFront;
			}
		}
	}

	template<typename T>
	inline void SList<T>::PopBack()
	{
		if (mSize > 1)
		{
			std::shared_ptr<Node> newBack = mFront;

			while (newBack->Next != mBack)
			{
				newBack = newBack->Next;
			}

			newBack->Next = nullptr;
			mBack = newBack;
			mSize--;
			
			if (mSize == 1)
			{
				mFront = mBack;
			}
		}
		else if (mSize == 1)
		{
			mFront = nullptr;
			mBack = nullptr;
			mSize = 0;
		}
	}

	template<typename T>
	inline bool SList<T>::Remove(const T& value)
	{
		 return Remove(Find(value));
	}

	template<typename T>
	inline bool SList<T>::Remove(const Iterator& it)
	{
		if (it.mOwner != this)
		{
			throw std::runtime_error("Invalid iterator.");
		}

		bool isRemoved = false;

		if (it != end())
		{
			if (it.mNode == mBack)
			{
				PopBack();
			}
			else
			{
				std::shared_ptr<Node> next = it.mNode->Next;
				it.mNode->Data.~T();
				new(&it.mNode->Data)T(std::move(next->Data));
				it.mNode->Next = next->Next;

				if (it.mNode->Next == nullptr)
				{
					mBack = it.mNode;
				}

				--mSize;
			}

			isRemoved = true;
		}

		return isRemoved;
	}

	template<typename T>
	inline void SList<T>::Clear()
	{
		mSize = 0;
		mFront = nullptr;
		mBack = nullptr;
	}
#pragma endregion Modifiers
}
