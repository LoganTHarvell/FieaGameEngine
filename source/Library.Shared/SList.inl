#include "SList.h"

namespace Library
{
#pragma region Node
	template<typename T>
	template<typename... Args>
	inline Library::SList<T>::Node::Node(const std::shared_ptr<Node>& next, Args&&... args) :
		Next(next), Data(std::forward<Args>(args)...)
	{
	}
#pragma endregion Node

#pragma region Iterator
	template<typename T>
	inline SList<T>::Iterator::Iterator(const SList& owner, const std::shared_ptr<Node>& node) :
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
	inline SList<T>::ConstIterator::ConstIterator(const SList& owner, const std::shared_ptr<Node>& node) :
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

#pragma region Special Members
	template<typename T>
	inline SList<T>::SList(const EqualityFunctor& equalityFunctor) :
		mEqualityFunctor(std::make_shared<EqualityFunctor>(equalityFunctor))
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
	inline SList<T>::SList(std::initializer_list<T> rhs, const EqualityFunctor& equalityFunctor) :
		mEqualityFunctor(std::make_shared<EqualityFunctor>(equalityFunctor))
	{
		for (const auto& value : rhs)
		{
			PushBack(value);
		}
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(std::initializer_list<T> rhs)
	{
		Clear();

		for (const auto& value : rhs)
		{
			PushBack(value);
		}

		return *this;
	}
#pragma endregion Special Members

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
		if (!mEqualityFunctor || !*mEqualityFunctor)
		{
			throw std::runtime_error("EqualityFunctor null.");
		}
		auto it = begin();
		for (; it != end(); ++it)
		{
			if (mEqualityFunctor->operator()(*it, value))
			{
				break;
			}
		}

		return it;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::Find(const T& value) const
	{
		if (!mEqualityFunctor || !*mEqualityFunctor)
		{
			throw std::runtime_error("EqualityFunctor null.");
		}
		
		auto it = cbegin();
		for (; it != cend(); ++it)
		{
			if (mEqualityFunctor->operator()(*it, value))
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
	template<typename ...Args>
	inline T& SList<T>::EmplaceFront(Args&& ...args)
	{
		mFront = std::make_shared<Node>(mFront, std::forward<Args>(args)...);

		if (mSize == 0)
		{
			mBack = mFront;
		}

		mSize++;

		return mFront->Data;
	}

	template<typename T>
	inline void SList<T>::PushFront(const T& data)
	{
		EmplaceFront(data);
	}

	template<typename T>
	inline void SList<T>::PushFront(T&& data)
	{
		EmplaceFront(std::move(data));
	}

	template<typename T>
	template<typename... Args>
	inline T& SList<T>::EmplaceBack(Args&&... args)
	{
		auto newNode = std::make_shared<Node>(nullptr, std::forward<Args>(args)...);

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

		return newNode->Data;
	}

	template<typename T>
	inline void SList<T>::PushBack(const T& data)
	{
		EmplaceBack(data);
	}

	template<typename T>
	inline void SList<T>::PushBack(T&& data)
	{
		EmplaceBack(std::move(data));
	}

	template<typename T>
	template<typename ...Args>
	inline typename SList<T>::Iterator SList<T>::EmplaceAfter(const Iterator& position, Args&&... args)
	{
		if (this != position.mOwner)
		{
			throw std::runtime_error("Invalid iterator.");
		}

		if (position == end())
		{
			EmplaceBack(std::forward<Args>(args)...);
			return Iterator(*this, mBack);
		}

		auto newNode = std::make_shared<Node>(nullptr, std::forward<Args>(args)...);
		newNode->Next = position.mNode->Next;
		position.mNode->Next = newNode;

		return Iterator(*this, newNode);
	}
	
	template<typename T>
	inline typename SList<T>::Iterator SList<T>::InsertAfter(const Iterator& position, const T& data)
	{
		return EmplaceAfter(position, data);
	}
	
	template<typename T>
	inline typename SList<T>::Iterator SList<T>::InsertAfter(const Iterator& position, T&& data)
	{
		return EmplaceAfter(position, std::move(data));
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
