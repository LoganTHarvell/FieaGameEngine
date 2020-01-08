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

	template<typename T>
	inline SList<T>::SList(const SList& rhs)
	{
		std::shared_ptr<Node> currentNode = rhs.mFront;
		while (currentNode != nullptr)
		{
			PushBack(currentNode->Data);
			currentNode = currentNode->Next;
		}
	}

	template<typename T>
	inline SList<T>::SList(SList&& rhs) :
		mSize(rhs.mSize), mFront(rhs.mFront), mBack(rhs.mBack)
	{
		rhs.Size = 0;
		rhs.mFront = nullptr;
		rhs.mBack = nullptr;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(const SList<T>& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			std::shared_ptr<Node> currentNode = rhs.mFront;
			while (currentNode != nullptr)
			{
				PushBack(currentNode->Data);
				currentNode = currentNode->Next;
			}
		}

		return *this;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(SList&& rhs)
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
		if (mSize != rhs.mSize) return false;

		bool isEqual = true;

		std::shared_ptr<Node> currentNode = mFront;
		std::shared_ptr<Node> rhsCurrentNode = rhs.mFront;

		while (currentNode != nullptr)
		{
			if (currentNode->Data != rhsCurrentNode->Data)
			{
				isEqual = false;
				break;
			}

			currentNode = currentNode->Next;
			rhsCurrentNode = rhsCurrentNode->Next;
		}

		return isEqual;
	}

	template<typename T>
	inline bool SList<T>::operator!=(const SList& rhs) const noexcept
	{
		if (mSize != rhs.mSize) return true;

		bool isNotEqual = false;

		std::shared_ptr<Node> currentNode = mFront;
		std::shared_ptr<Node> rhsCurrentNode = rhs.mFront;

		while (currentNode != nullptr)
		{
			if (currentNode->Data != rhsCurrentNode->Data)
			{
				isNotEqual = true;
				break;
			}

			currentNode = currentNode->Next;
			rhsCurrentNode = rhsCurrentNode->Next;
		}

		return isNotEqual;
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
	inline void SList<T>::PushFront(const T& data)
	{
		mFront = std::make_shared<Node>(data, mFront);
		
		if (IsEmpty())
		{
			mBack = mFront;
		}

		mSize++;
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
	inline void SList<T>::PushBack(const T& data)
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
	inline void SList<T>::Clear()
	{
		mSize = 0;
		mFront = nullptr;
		mBack = nullptr;
	}
}