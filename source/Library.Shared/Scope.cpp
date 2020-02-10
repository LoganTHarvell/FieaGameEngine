#include "pch.h"

#include "Scope.h"

#include "Utility.h"

namespace Library
{
#pragma region Constructors, Destructor, Assignment
	Scope::Scope(const std::size_t capacity) :
		mTable(TableType(Math::FindNextPrime(capacity))), mPairPtrs(capacity)
	{
	}

	Scope::~Scope()
	{
		Clear();
	}

	Scope::Scope(const Scope& rhs) :
		mParent(rhs.mParent), mTable(rhs.mTable.BucketCount()), mPairPtrs(rhs.mPairPtrs.Size())
	{
		for (auto& tableEntryPtr : rhs.mPairPtrs)
		{
			if (tableEntryPtr->second.Type() == DataType::Types::Scope)
			{
				DataType data;
				data.SetType(DataType::Types::Scope);
				
				for (std::size_t i = 0; i < tableEntryPtr->second.Size(); ++i)
				{
					mChildren.PushBack(tableEntryPtr->second[i]);
					data.PushBack(&mChildren.Back());
				}

				mPairPtrs.PushBack(&(*mTable.Insert({ tableEntryPtr->first, data }).first));
			}
			else
			{
				mPairPtrs.PushBack(&(*mTable.Insert(*tableEntryPtr).first));
			}
		}
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		Clear();

		mParent = rhs.mParent;
		mTable = TableType(rhs.mTable.BucketCount());
		
		mPairPtrs.ShrinkToFit();
		mPairPtrs.Reserve(rhs.mPairPtrs.Size());

		for (auto& tableEntryPtr : rhs.mPairPtrs)
		{
			if (tableEntryPtr->second.Type() == DataType::Types::Scope)
			{
				DataType data;
				data.SetType(DataType::Types::Scope);
				
				for (std::size_t i = 0; i < tableEntryPtr->second.Size(); ++i)
				{
					mChildren.PushBack(tableEntryPtr->second[i]);
					data.PushBack(&mChildren.Back());
				}

				mPairPtrs.PushBack(&(*mTable.Insert({ tableEntryPtr->first, data }).first));
			}
			else
			{
				mPairPtrs.PushBack(&(*mTable.Insert(*tableEntryPtr).first));
			}
		}

		return *this;
	}

	Scope::Scope(Scope&& rhs) noexcept :
		mParent(rhs.mParent), mTable(std::move(rhs.mTable)), mPairPtrs(std::move(rhs.mPairPtrs)), mChildren(std::move(rhs.mChildren))
	{
		rhs.mParent = nullptr;
	}

	Scope& Scope::operator=(Scope&& rhs) noexcept
	{
		Clear();

		mParent = rhs.mParent;
		mTable = std::move(rhs.mTable);
		mPairPtrs = std::move(rhs.mPairPtrs);
		mChildren = std::move(rhs.mChildren);

		rhs.mParent = nullptr;

		return *this;
	}

	Scope::Scope(const std::initializer_list<TableEntryType> rhs, const std::size_t capacity) :
		mTable(TableType(Math::FindNextPrime(capacity))), mPairPtrs(capacity)
	{
		for (auto& tableEntry : rhs)
		{
			if (tableEntry.second.Type() == DataType::Types::Scope)
			{
				DataType data;
				data.SetType(DataType::Types::Scope);

				for (std::size_t i = 0; i < tableEntry.second.Size(); ++i)
				{
					mChildren.PushBack(tableEntry.second[i]);
					data.PushBack(&mChildren.Back());
				}

				mPairPtrs.PushBack(&(*mTable.Insert({ tableEntry.first, data }).first));
			}
			else
			{
				mPairPtrs.PushBack(&(*mTable.Insert(tableEntry).first));
			}
		}
	}

	Scope& Scope::operator=(const std::initializer_list<TableEntryType> rhs)
	{
		Clear();
		
		mTable = TableType(Math::FindNextPrime(rhs.size()));

		mPairPtrs.ShrinkToFit();
		mPairPtrs.Reserve(rhs.size());

		for (auto& tableEntry : rhs)
		{
			if (tableEntry.second.Type() == DataType::Types::Scope)
			{
				DataType data;
				data.SetType(DataType::Types::Scope);

				for (std::size_t i = 0; i < tableEntry.second.Size(); ++i)
				{
					mChildren.PushBack(tableEntry.second[i]);
					data.PushBack(&mChildren.Back());
				}

				mPairPtrs.PushBack(&(*mTable.Insert({ tableEntry.first, data }).first));
			}
			else
			{
				mPairPtrs.PushBack(&(*mTable.Insert(tableEntry).first));
			}
		}

		return *this;
	}
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
	bool Scope::operator==(const Scope& rhs) const noexcept
	{
		if (this == &rhs)								return true;
		if (mPairPtrs.Size() != rhs.mPairPtrs.Size())	return false;

		for (std::size_t i = 0; i < mPairPtrs.Size(); ++i)
		{
			if (*mPairPtrs[i] != *rhs.mPairPtrs[i]) return false;
		}

		return true;
	}

	bool Scope::operator!=(const Scope& rhs) const noexcept
	{
		return !(operator==(rhs));
	}
#pragma endregion Boolean Operators

#pragma region Element Accessors
	Scope* Scope::GetParent()
	{
		return mParent;
	}

	const Scope* Scope::GetParent() const
	{
		return mParent;
	}

	Scope::DataType& Scope::operator[](const NameType name)
	{
		return Append(name);
	}

	const Scope::DataType& Scope::operator[](const NameType name) const
	{
		const DataType* entry = Find(name);
		if (entry == nullptr) throw std::runtime_error("Name not found.");

		return *entry;
	}

	Scope::DataType& Scope::operator[](const std::size_t index)
	{
		return mPairPtrs[index]->second;
	}

	const Scope::DataType& Scope::operator[](const std::size_t index) const
	{
		return mPairPtrs[index]->second;
	}

	Scope::DataType* Scope::Find(const NameType name)
	{
		TableType::Iterator it = mTable.Find(name);
		return it != mTable.end() ? &it->second : nullptr;
	}

	const Scope::DataType* Scope::Find(const NameType name) const
	{
		TableType::ConstIterator it = mTable.Find(name);
		return it != mTable.end() ? &it->second : nullptr;
	}

	const std::string* Scope::FindName(const std::size_t index) const
	{
		return index < mPairPtrs.Size() ? &mPairPtrs[index]->first : nullptr;
	}

	Scope::DataType* Scope::Search(const std::string name, Scope** scopePtrOut)
	{
		DataType* result = Find(name);

		if (result)
		{
			if (scopePtrOut) *scopePtrOut = this;
			return result;
		}

		if (mParent) return mParent->Search(name, scopePtrOut);
		
		if (scopePtrOut) *scopePtrOut = nullptr;
		return nullptr;
	}

	Scope::DataType* Scope::SearchChildren(const std::string name, Scope** scopePtrOut)
	{
		Vector queue = { this };
		return SearchChildrenHelper(queue, name, scopePtrOut);
	}
#pragma endregion Element Accessors

#pragma region Modifiers
	Scope::DataType& Scope::Append(const NameType name)
	{
		auto [it, isNew] = mTable.Insert({ name, DataType() });
		if (isNew) mPairPtrs.PushBack(&(*it));

		return it->second;
	}

	Scope& Scope::AppendScope(const NameType name)
	{
		Scope child;
		child.mParent = this;
		mChildren.PushBack(child);

		auto [it, isNew] = mTable.Insert({ name, DataType(&mChildren.Back()) });
		if (isNew) mPairPtrs.PushBack(&(*it));
		else it->second.PushBack(&mChildren.Back());

		return *(it->second.Back<DataType::ScopePointer>());
	}

	void Scope::Clear()
	{
		mTable.Clear();
		mPairPtrs.Clear();
		mChildren.Clear();
	}
#pragma endregion Modifiers

#pragma region Helper Methods
	Scope::DataType* Scope::SearchChildrenHelper(const Vector<Scope*>& queue, const std::string name, Scope** scopePtrOut)
	{
		Vector<Scope*> newQueue;

		for (auto& scopePtr : queue)
		{
			DataType* result = scopePtr->Find(name);

			if (result)
			{
				if (scopePtrOut) *scopePtrOut = scopePtr;
				return result;
			}

			newQueue.Reserve(scopePtr->mChildren.Size());

			for (auto& child : scopePtr->mChildren)
			{
				newQueue.PushBack(&child);
			}
		}

		if (newQueue.Size() > 0) return SearchChildrenHelper(newQueue, name, scopePtrOut);

		if (scopePtrOut) *scopePtrOut = nullptr;
		return nullptr;
	}
#pragma endregion Helper Methods
}