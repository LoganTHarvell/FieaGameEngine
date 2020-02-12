#include "pch.h"

#include "Scope.h"

#include "Utility.h"

namespace Library
{
	RTTI_DEFINITIONS(Scope);

#pragma region Constructors, Destructor, Assignment
	Scope::Scope(const std::size_t capacity) :
		mTable(TableType(std::max(TableType::DefaultBucketCount, Math::FindNextPrime(capacity)))), mPairPtrs(capacity)
	{
	}

	Scope::~Scope()
	{
		Clear();
	}

	Scope::Scope(const Scope& rhs) :
		mTable(rhs.mTable.BucketCount()), mPairPtrs(rhs.mPairPtrs.Size())
	{
		for (auto& tableEntryPtr : rhs.mPairPtrs)
		{
			if (tableEntryPtr->second.Type() == DataType::Types::Scope)
			{
				DataType data;
				data.SetType(DataType::Types::Scope);
				
				for (std::size_t i = 0; i < tableEntryPtr->second.Size(); ++i)
				{
					mChildren.PushBack(new Scope(tableEntryPtr->second[i]));
					mChildren.Back()->mParent = this;
					data.PushBack(mChildren.Back());
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
					mChildren.PushBack(new Scope(tableEntryPtr->second[i]));
					mChildren.Back()->mParent = this;
					data.PushBack(mChildren.Back());
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
		for (auto& child : mChildren)
		{
			child->mParent = this;
		}

		if (rhs.mParent)
		{
			rhs.mParent->mChildren.PushBack(this);

			auto [data, index] = rhs.mParent->FindScope(rhs);
			data->Set(this, index);
			delete &rhs;
		}
	}

	Scope& Scope::operator=(Scope&& rhs) noexcept
	{
		Clear();
		
		Scope* lhsParent = mParent;

		mParent = rhs.mParent;
		mTable = std::move(rhs.mTable);
		mPairPtrs = std::move(rhs.mPairPtrs);
		mChildren = std::move(rhs.mChildren);

		for (auto& child : mChildren)
		{
			child->mParent = this;
		}

		if (rhs.mParent)
		{
			rhs.mParent->mChildren.PushBack(this);

			auto [data, index] = rhs.mParent->FindScope(rhs);
			data->Set(this, index);
			delete& rhs;

 			if (lhsParent) lhsParent->Orphan(*this);
		}

		return *this;
	}

	Scope::Scope(const std::initializer_list<TableEntryType> rhs, const std::size_t capacity) :
		mTable(TableType(std::max(TableType::DefaultBucketCount, Math::FindNextPrime(capacity)))), mPairPtrs(capacity)
	{
		for (auto& tableEntry : rhs)
		{
			if (tableEntry.second.Type() == DataType::Types::Scope)
			{
				DataType data;
				data.SetType(DataType::Types::Scope);

				for (std::size_t i = 0; i < tableEntry.second.Size(); ++i)
				{
					mChildren.PushBack(new Scope(tableEntry.second[i]));
					mChildren.Back()->mParent = this;
					data.PushBack(mChildren.Back());
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
					mChildren.PushBack(new Scope(tableEntry.second[i]));
					mChildren.Back()->mParent = this;
					data.PushBack(mChildren.Back());
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

		for (const auto& pairPtr : mPairPtrs)
		{
			const DataType* rhsData = Find(pairPtr->first);
			if (!rhsData || pairPtr->second != *rhsData) return false;
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

	Scope::DataType& Scope::operator[](const NameType& name)
	{
		return Append(name);
	}

	const Scope::DataType& Scope::operator[](const NameType& name) const
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

	Scope::DataType* Scope::Find(const NameType& name)
	{
		TableType::Iterator it = mTable.Find(name);
		return it != mTable.end() ? &it->second : nullptr;
	}

	const Scope::DataType* Scope::Find(const NameType& name) const
	{
		TableType::ConstIterator it = mTable.Find(name);
		return it != mTable.end() ? &it->second : nullptr;
	}

	const std::string* Scope::FindName(const std::size_t index) const
	{
		return index < mPairPtrs.Size() ? &mPairPtrs[index]->first : nullptr;
	}

	std::pair<Scope::DataType*, std::size_t> Scope::FindScope(const Scope& scope)
	{
		for (auto& pairPtr : mPairPtrs)
		{
			if (pairPtr->second.Type() == DataType::Types::Scope)
			{
				for (std::size_t i = 0; i < pairPtr->second.Size(); ++i)
				{
					if (&scope == pairPtr->second.Get<DataType::ScopePointer>(i))
					{
						return { &pairPtr->second, i };
					}
				}
			}
		}

		return { nullptr, 0 };
	}

	std::pair<const Scope::DataType*, std::size_t> Scope::FindScope(const Scope& scope) const
	{
		for (auto& pairPtr : mPairPtrs)
		{
			if (pairPtr->second.Type() == DataType::Types::Scope)
			{
				for (std::size_t i = 0; i < pairPtr->second.Size(); ++i)
				{
					if (&scope == pairPtr->second.Get<DataType::ScopePointer>(i))
					{
						return { &pairPtr->second, i };
					}
				}
			}
		}

		return { nullptr, 0 };
	}

	Scope::DataType* Scope::Search(const NameType& name, Scope** scopePtrOut)
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

	const Scope::DataType* Scope::Search(const NameType& name, const Scope** scopePtrOut) const
	{
		const DataType* result = Find(name);

		if (result)
		{
			if (scopePtrOut) *scopePtrOut = this;
			return result;
		}

		if (mParent) return mParent->Search(name, scopePtrOut);
		
		if (scopePtrOut) *scopePtrOut = nullptr;
		return nullptr;
	}

	Scope::DataType* Scope::SearchChildren(const NameType& name, Scope** scopePtrOut)
	{
		Vector queue = { this };
		return SearchChildrenHelper(queue, name, scopePtrOut);
	}

	const Scope::DataType* Scope::SearchChildren(const NameType& name, const Scope** scopePtrOut) const
	{
		Scope* nonConstThis = const_cast<Scope*>(this);
		Vector queue = { nonConstThis };
		return nonConstThis->SearchChildrenHelper(queue, name, const_cast<Scope**>(scopePtrOut));
	}
#pragma endregion Element Accessors

#pragma region Modifiers
	Scope::DataType& Scope::Append(const NameType& name)
	{
		if (name.empty()) throw std::runtime_error("Name cannot be empty.");

		auto [it, isNew] = mTable.Insert({ name, DataType() });
		if (isNew) mPairPtrs.PushBack(&(*it));

		return it->second;
	}

	Scope& Scope::AppendScope(const NameType& name)
	{
		if (name.empty()) throw std::runtime_error("Name cannot be empty.");

		Scope* child = new Scope;
		child->mParent = this;
		mChildren.PushBack(child);

		auto [it, isNew] = mTable.Insert({ name, DataType(mChildren.Back()) });
		if (isNew) mPairPtrs.PushBack(&(*it));
		else it->second.PushBack(mChildren.Back());

		return *(it->second.Back<DataType::ScopePointer>());
	}

	Scope* Scope::Orphan(Scope& child)
	{
		auto [data, index] = FindScope(child);

		if (data)
		{
			data->RemoveAt(index);
			mChildren.Remove(&child);

			return &child;
		}

		return nullptr;
	}

	Scope& Scope::Adopt(Scope& child, const NameType& name)
	{
		if (this == child.mParent) return child;

		Scope* orphan = child.mParent->Orphan(child);
		orphan->mParent = this;
		mChildren.PushBack(orphan);

		auto [it, isNew] = mTable.Insert({ name, DataType(mChildren.Back()) });
		if (isNew) mPairPtrs.PushBack(&(*it));
		else it->second.PushBack(mChildren.Back());
		
		return *mChildren.Back();
	}

	void Scope::Clear()
	{
		mTable.Clear();
		mPairPtrs.Clear();

		for (auto& child : mChildren)
		{
			if (child != nullptr)
			{
				delete child;
				child = nullptr;
			}
		}

		mChildren.Clear();
	}
#pragma endregion Modifiers

#pragma region Helper Methods
	Scope::DataType* Scope::SearchChildrenHelper(const Vector<Scope*>& queue, const NameType& name, Scope** scopePtrOut)
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
				newQueue.PushBack(child);
			}
		}

		if (newQueue.Size() > 0) return SearchChildrenHelper(newQueue, name, scopePtrOut);

		if (scopePtrOut) *scopePtrOut = nullptr;
		return nullptr;
	}
#pragma endregion Helper Methods
}