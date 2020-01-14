#include "SList.h"

namespace Library
{
#pragma region Node
	template<typename T>
	inline SList<T>::Node::Node(const T& data, std::shared_ptr<Node> next) :
		Data(data), Next(next)
	{
	}
#pragma endregion Node

#pragma region Iterator
	template<typename T>
	inline SList<T>::Iterator::Iterator(const SList<T>& list, std::shared_ptr<Node> node) :
		mOwner(&list), mNode(node)
	{
	}

	template<typename T>
	inline T& SList<T>::Iterator::operator*() const
	{
		if (mNode == nullptr)
		{
			throw std::runtime_error("Iterator has no associated node.");
		}

		return mNode->Data;
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
		if (mNode == nullptr)
		{
			throw std::runtime_error("Cannot go past the end of list.");
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
	inline SList<T>::ConstIterator::ConstIterator(const Iterator& rhs) :
		mOwner(rhs.mOwner), mNode(rhs.mNode)
	{
	}

	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const SList& list, std::shared_ptr<Node> node) :
		mOwner(&list), mNode(node)
	{
	}

	template<typename T>
	inline const T& SList<T>::ConstIterator::operator*() const
	{
		if (mNode == nullptr)
		{
			throw std::runtime_error("ConstIterator has no associated node.");
		}

		return mNode->Data;
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
		if (mNode == nullptr)
		{
			throw std::runtime_error("Cannot go past the end of list.");
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

#pragma region SList
	template<typename T>
	inline SList<T>::SList(const std::initializer_list<T> list)
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template<typename T>
	inline SList<T>::SList(const SList& rhs)
	{
		for (const auto& value : rhs)
		{
			PushBack(value);
		}
	}

	template<typename T>
	inline SList<T>::SList(SList&& rhs) noexcept :
		mSize(rhs.mSize), mFront(rhs.mFront), mBack(rhs.mBack)
	{
		rhs.mSize = 0;
		rhs.mFront = nullptr;
		rhs.mBack = nullptr;
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

	template<typename T>
	inline SList<T>& SList<T>::operator=(const SList<T>& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			for (const auto& value : rhs)
			{
				PushBack(value);
			}
		}

		return *this;
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

			rhs.mSize = 0;
			rhs.mFront = nullptr;
			rhs.mBack = nullptr;
		}

		return *this;
	}

	template<typename T>
	inline SList<T>::~SList()
	{
		Clear();
	}

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

	template<typename T>
	inline size_t SList<T>::Size() const
	{
		return mSize;
	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return mSize == 0;
	}

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
	inline T& SList<T>::Front()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}

		return mFront->Data;
	}

	template<typename T>
	inline const T& SList<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}

		return mFront->Data;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::PushFront(const T& data)
	{
		mFront = std::make_shared<Node>(data, mFront);
		
		if (IsEmpty())
		{
			mBack = mFront;
		}

		mSize++;
		
		return Iterator(*this, mFront);
	}

	template<typename T>
	inline void SList<T>::PopFront()
	{
		if (!IsEmpty())
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
	inline T& SList<T>::Back()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}

		return mBack->Data;
	}
	
	template<typename T>
	inline const T& SList<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}

		return mBack->Data;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::PushBack(const T& data)
	{
		std::shared_ptr<Node> newNode = std::make_shared<Node>(data);

		if (IsEmpty())
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

		return Iterator(*this, mBack);
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
	inline typename SList<T>::Iterator SList<T>::Find(const T& value, std::function<bool(T, T)> equal)
	{
		for (auto it = begin(); it != end(); ++it)
		{
			if (equal(*it, value))
			{
				return it;
			}
		}

		return Iterator();
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::Find(const T& value, std::function<bool(T, T)> equal) const
	{
		for (auto it = begin(); it != end(); ++it)
		{
			if (equal(*it, value))
			{
				return ConstIterator(it);
			}
		}

		return ConstIterator();
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::InsertAfter(const Iterator& iterator, const T& data)
	{
		if (this != iterator.mOwner)
		{
			throw std::runtime_error("Iterator not associated with this list.");
		}

		if (iterator == end())
		{
			return PushBack(data);
		}
		
		std::shared_ptr<Node> newNode = std::make_shared<Node>(data);
		newNode->Next = iterator.mNode->Next;
		iterator.mNode->Next = newNode;

		return Iterator(*this, newNode);
	}

	template<typename T>
	inline bool SList<T>::Remove(const T& value, std::function<bool(T, T)> equal)
	{
		SList<T>::Iterator it = Find(value, equal);

		bool isRemoved = false;

		if (it != Iterator())
		{
			isRemoved = Remove(it);
		}

		return isRemoved;
	}

	template<typename T>
	inline bool SList<T>::Remove(const Iterator& iterator)
	{
		if (iterator.mOwner != this)
		{
			throw std::runtime_error("Iterator not associated with this list.");
		}

		bool isRemoved = false;

		if (iterator != end())
		{
			if (iterator.mNode == mBack)
			{
				PopBack();
			}
			else
			{
				std::shared_ptr<Node> next = iterator.mNode->Next;
				iterator.mNode->Data.~T();
				new(&iterator.mNode->Data)T(std::move(next->Data));
				iterator.mNode->Next = next->Next;

				if (iterator.mNode->Next == nullptr)
				{
					mBack = iterator.mNode;
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
}
#pragma endregion SList